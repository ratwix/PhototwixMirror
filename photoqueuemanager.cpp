#include <QTimer>

#include "photoqueuemanager.h"
#include "photoqueuetwitter.h"
#include "photoqueuemirror.h"

#include "rapidjson/document.h"
#include "clog.h"

PhotoQueueManager::PhotoQueueManager(QObject *parent) : QObject(parent)
{
    m_canPop = true;
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

PhotoQueueManager::PhotoQueueManager(Parameters *parameter, QObject *parent)  : QObject(parent)
{
    m_nbPhotoInQueue = 0;
    m_parameter = parameter;
    m_canPopTimeout = new QTimer(this);
    QQmlEngine::setObjectOwnership(m_canPopTimeout, QQmlEngine::CppOwnership);
    m_canPopTimeout->setSingleShot(true);
    connect(m_canPopTimeout, SIGNAL(timeout()), this, SLOT(setCanPopTrue()));
    m_canPop = true;
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

PhotoQueueManager::~PhotoQueueManager()
{
    CLog::Write(CLog::Debug, "Delete PhotoQueueManager");
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

void PhotoQueueManager::pushMirror(QString name, QString json)
{
    PhotoQueueMirror *photo = new PhotoQueueMirror();

    photo->setName(name);
    photo->setJson(json);
    //Parse JSON
    Document document;
    document.Parse(json.toStdString().c_str());

    if (!document.HasMember("photoProcessResult")) {
        CLog::Write(CLog::Error, "JSON didn't have photoProcessResult");
        return;
    }

    const Value& result = document["photoProcessResult"];

    if (result.HasMember("templateName")) {
        QString templateName = QString(result["templateName"].GetString());
        bool found = false;
        for (int i = 0; i < m_parameter->getActivesTemplates().length(); i++) {
            if (Template *t = dynamic_cast<Template*>(m_parameter->getActivesTemplates()[i])) {
                if (t->getName() == templateName) {
                    found = true;
                    photo->setUsedTemplate(t);
                    break;
                }
            }else {
                CLog::Write(CLog::Fatal, "Bad type QObject -> Template");
            }
        }
        if (!found) {
            CLog::Write(CLog::Fatal, "Template from name not found in active template");
        }
    }

    if (result.HasMember("effectName")) {
        photo->setEffect(QString(result["effectName"].GetString()));
    }

    if (result.HasMember("nbPhoto")) {
        photo->setNbPhotos(result["nbPhoto"].GetInt());
    }

    if (result.HasMember("photosPath")) {
        photo->setPhotoPath(QString(result["photosPath"].GetString()));
    }

    if (result.HasMember("clientIP")) {
        photo->setClientIP(QString(result["clientIP"].GetString()));
    }

    if (result.HasMember("photos")) {
        const Value& photos = result["photos"];
        if (photos.IsArray()) {
            for (SizeType i = 0; i < photos.Size(); i++) {
                Value const &value = photos[i];
                if (value.HasMember("fileName")) {
                    QString fn = QString(value["fileName"].GetString());
                    photo->photosList()->append(fn);
                }
            }
        }
    }
    int tmp = photo->photosList()->length();
    //Push front, les photos du photomaton sont prioritaires
    m_photoQueueList.push_front(photo);
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
            setCanPop(false);
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
            //If it is a mirror
            if (PhotoQueueMirror *photoMirror = dynamic_cast<PhotoQueueMirror*>(photo)) {
                m_parameter->getPhotogallery()->addPhotoMirror(
                            photoMirror->name(),
                            photoMirror->usedTemplate(),
                            photoMirror->effect(),
                            photoMirror->clientIP(),
                            photoMirror->photoPath(),
                            photoMirror->photosList()
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
