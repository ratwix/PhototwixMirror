#include <QTimer>

#include "photoqueuemanager.h"
#include "photoqueuetwitter.h"

PhotoQueueManager::PhotoQueueManager(QObject *parent) : QObject(parent)
{
    m_canPop = true;
}

PhotoQueueManager::PhotoQueueManager(Parameters *parameter, QObject *parent)  : QObject(parent)
{
    m_nbPhotoInQueue = 0;
    m_parameter = parameter;
    m_canPopTimeout = new QTimer(this);
    m_canPopTimeout->setSingleShot(true);
    connect(m_canPopTimeout, SIGNAL(timeout()), this, SLOT(setCanPopTrue()));
    m_canPop = true;
}

/**
 * @brief PhotoQueueManager::pushTwitter
 *  Push a twitter new photo
 */
void PhotoQueueManager::pushTwitter(QString name,
                               Template *t,
                               QString effectName,
                               QString date,
                               QString twitterId,
                               QString twitterMessage,
                               QString twitterProfileName,
                               QString twitterProfileImage,
                               QUrl twitterPhotoSource)
{
    PhotoQueueTwitter *photo = new PhotoQueueTwitter();
    photo->setName(name);
    photo->setUsedTemplate(t);
    photo->setEffect(effectName);
    photo->setDate(date);
    photo->setTwitId(twitterId);
    photo->setTwitText(twitterMessage);
    photo->setTwitProfileName(twitterProfileName);
    photo->setTwitProfileImage(twitterProfileImage);
    photo->setTwitMediaUrl(twitterPhotoSource);

    m_photoQueueList.push_back(photo);
    setNbPhotoInQueue(m_photoQueueList.length());
}


/**
 * @brief PhotoQueueManager::pushTwitter
 * Pop a photo and add it to the gallery
 */

void PhotoQueueManager::pop()
{
    if (canPop()) {
        if (!m_photoQueueList.isEmpty()) {
            setCanPop(false); //TODO: add timer to setPop to true again after timeout
            PhotoQueue *photo = m_photoQueueList.first();
            m_photoQueueList.pop_front();

            //If it is a twitter
            if (PhotoQueueTwitter *photoTwitter = dynamic_cast<PhotoQueueTwitter*>(photo)) {
                m_parameter->getPhotogallery()->addPhotoTwitter(
                            photoTwitter->name(),
                            photoTwitter->usedTemplate(),
                            photoTwitter->effect(),
                            photoTwitter->twitText(),
                            photoTwitter->twitProfileName(),
                            photoTwitter->twitMediaUrl()
                );
            }
            setNbPhotoInQueue(m_photoQueueList.length());
            delete photo;
        }
    }
}

bool PhotoQueueManager::canPop() const
{
    return m_canPop;
}

void PhotoQueueManager::setCanPop(bool canPop)
{
   //CanPop timeout
   if (!canPop) {
       m_canPopTimeout->start(10000); //After 10 second, timeout popoup, we can pop again
   } else {
       m_canPopTimeout->stop();
   }

   m_canPop = canPop;
   emit canPopChanged();
}

int PhotoQueueManager::nbPhotoInQueue() const
{
    return m_nbPhotoInQueue;
}

void PhotoQueueManager::setNbPhotoInQueue(int nbPhotoInQueue)
{
    m_nbPhotoInQueue = nbPhotoInQueue;
    emit nbPhotoInQueueChanged();
}

void PhotoQueueManager::setCanPopTrue()
{
    m_canPop = true;
}
