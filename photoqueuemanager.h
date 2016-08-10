#ifndef PHOTOQUEUEMANAGER_H
#define PHOTOQUEUEMANAGER_H

#include <QObject>
#include <QTimer>

#include "parameters.h"
#include "photoqueue.h"

/**
 * @brief The PhotoQueueManager class contains all the Photo that has been
 * recieved by social media, and has not been added to the gallery yet.
 * It act as a buffer.
 *
 * This is used to manage multiple sender at the same time/
 *
 * The QML part retrieve one by one the PhotoQueue in the PhotoQueueManager list
 */

class PhotoQueue;
class PhotoQueueManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int nbPhotoInQueue READ nbPhotoInQueue WRITE setNbPhotoInQueue NOTIFY nbPhotoInQueueChanged)
    Q_PROPERTY(int canPop READ canPop WRITE setCanPop NOTIFY canPopChanged)
public:
    explicit PhotoQueueManager(QObject *parent = 0);
    PhotoQueueManager(Parameters *parameter, QObject *parent = 0);

    Q_INVOKABLE void pushTwitter(QString name,
                            Template *t,
                            QString effectName,
                            QString date,
                            QString twitterId,
                            QString twitterMessage,
                            QString twitterProfileName,
                            QString twitterProfileImage,
                            QUrl twitterPhotoSource);

    //TODO: pushMirror(Template *t,QString effectName, image list);

    Q_INVOKABLE void pop();

    bool canPop() const;
    void setCanPop(bool canPop);

    int nbPhotoInQueue() const;
    void setNbPhotoInQueue(int nbPhotoInQueue);

private:
    QList<PhotoQueue *>     m_photoQueueList;
    Parameters *            m_parameter;
    bool                    m_canPop;
    QTimer *                m_canPopTimeout;
    int                     m_nbPhotoInQueue;
signals:
    void nbPhotoInQueueChanged();
    void canPopChanged();
public slots:
    void setCanPopTrue();

};

#endif // PHOTOQUEUEMANAGER_H
