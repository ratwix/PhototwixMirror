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

    QString photoPath() const;
    void setPhotoPath(const QString &photoPath);

    QString clientIP() const;
    void setClientIP(const QString &clientIP);

    QList<QString> *photosList() const;
    void setPhotosList(QList<QString> *photosList);

private:
    QString         m_json;
    int             m_nbPhotos;
    QList<QString>  *m_photosList;
    QString         m_photoPath;
    QString         m_clientIP;
};

#endif // PHOTOQUEUEMIRROR_H
