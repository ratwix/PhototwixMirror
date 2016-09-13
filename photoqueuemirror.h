#ifndef PHOTOQUEUEMIRROR_H
#define PHOTOQUEUEMIRROR_H

#include "photoqueue.h"

class PhotoQueueMirror : public PhotoQueue
{
public:
    PhotoQueueMirror();
    virtual ~PhotoQueueMirror();

    QString json() const;
    void setJson(const QString &json);

    int nbPhotos() const;
    void setNbPhotos(int nbPhotos);

    QString mirrorIP() const;
    void setMirrorIP(const QString &mirrorIP);

    QList<QString> photosList() const;
    void setPhotosList(const QList<QString> &photosList);

    QString photoPath() const;
    void setPhotoPath(const QString &photoPath);

private:
    QString         m_json;
    int             m_nbPhotos;
    QString         m_mirrorIP;
    QList<QString>  m_photosList;
    QString         m_photoPath;
};

#endif // PHOTOQUEUEMIRROR_H
