#include "photoqueuemirror.h"

PhotoQueueMirror::PhotoQueueMirror()
{
    m_photosList = new QList<QString>();
}

PhotoQueueMirror::~PhotoQueueMirror()
{

}

QString PhotoQueueMirror::json() const
{
    return m_json;
}

void PhotoQueueMirror::setJson(const QString &json)
{
    m_json = json;
}

int PhotoQueueMirror::nbPhotos() const
{
    return m_nbPhotos;
}

void PhotoQueueMirror::setNbPhotos(int nbPhotos)
{
    m_nbPhotos = nbPhotos;
}



QString PhotoQueueMirror::photoPath() const
{
    return m_photoPath;
}

void PhotoQueueMirror::setPhotoPath(const QString &photoPath)
{
    m_photoPath = photoPath;
}

QString PhotoQueueMirror::clientIP() const
{
    return m_clientIP;
}

void PhotoQueueMirror::setClientIP(const QString &clientIP)
{
    m_clientIP = clientIP;
}

QList<QString> *PhotoQueueMirror::photosList() const
{
    return m_photosList;
}

void PhotoQueueMirror::setPhotosList(QList<QString> *photosList)
{
    m_photosList = photosList;
}
