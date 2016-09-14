#include "photoqueuemirror.h"

PhotoQueueMirror::PhotoQueueMirror()
{

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

QString PhotoQueueMirror::mirrorIP() const
{
    return m_mirrorIP;
}

void PhotoQueueMirror::setMirrorIP(const QString &mirrorIP)
{
    m_mirrorIP = mirrorIP;
}

QList<QString> PhotoQueueMirror::photosList() const
{
    return m_photosList;
}

void PhotoQueueMirror::setPhotosList(const QList<QString> &photosList)
{
    m_photosList = photosList;
}

QString PhotoQueueMirror::photoPath() const
{
    return m_photoPath;
}

void PhotoQueueMirror::setPhotoPath(const QString &photoPath)
{
    m_photoPath = photoPath;
}
