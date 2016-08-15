#ifndef PHOTOGALLERY_H
#define PHOTOGALLERY_H

#include <QObject>
#include "template.h"
#include "photo.h"
#include "parameters.h"

class Photo;
class PhotoGallery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> photoList READ photoList WRITE setPhotoList NOTIFY photoListChanged)
    Q_PROPERTY(int currentCopy READ currentCopy WRITE setCurrentCopy NOTIFY currentCopyChanged)
    Q_PROPERTY(int totalFileNumber READ totalFileNumber WRITE setTotalFileNumber NOTIFY totalFileNumberChanged)
    Q_PROPERTY(qint64 totalFileSize READ totalFileSize WRITE setTotalFileSize NOTIFY totalFileSizeChanged)
public:
    PhotoGallery();
    PhotoGallery(Parameters *parameter);
    ~PhotoGallery();
    Q_INVOKABLE Photo* addPhoto(QString name, Template *t);
    Q_INVOKABLE Photo* addPhotoTwitter(QString name,
                                Template *t,
                                QString effectName,
                                QString twitterMessage,
                                QString twitterProfileName,
                                QUrl twitterPhotoSource);
    Q_INVOKABLE void    prependPhoto(Photo *photo);

    Q_INVOKABLE void   removePhoto(QString name);

    Q_INVOKABLE void   saveGallery(QUrl destPath, bool saveSingle, bool saveDeleted, bool saveDeletedSingle);
    Q_INVOKABLE void   updateGalleryDiskSize(bool saveSingle, bool saveDeleted, bool saveDeletedSingle);

    Q_INVOKABLE void Serialize();
                void Unserialize(QList<QObject*> &templates);
    QList<QObject*> photoList() const;
    void setPhotoList(const QList<QObject *> &photoList);

    QUrl applicationDirPath() const;
    void setApplicationDirPath(const QUrl &applicationDirPath);

    int currentCopy() const;
    void setCurrentCopy(int currentCopy);

    qint64 totalFileSize() const;
    void setTotalFileSize(const qint64 &totalFileSize);

    int totalFileNumber() const;
    void setTotalFileNumber(int totalFileNumber);

private:
    void            addPhoto(Value const &value, QList<QObject*> &templates);

    void            saveGalleryThread(QUrl destPath, bool saveSingle, bool saveDeleted, bool saveDeletedSingle);

    QList<QObject*> m_photoList; //List of all photos in Gallery. List of Photo class
    QUrl            m_applicationDirPath;
    int             m_currentCopy;
    qint64          m_totalFileSize;
    int             m_totalFileNumber;
    //std::thread     m_t;

    Parameters      *m_parameter;

signals:
    void photoListChanged();
    void currentCopyChanged();
    void totalFileSizeChanged();
    void totalFileNumberChanged();
    void copyProgress(int current, int total);
    void copyStart();
    void copyEnd();
    void showPhoto(Photo *photo);

public slots:
    void            imageDownloaded(Photo *photo);
};

#endif // GALLERY_H
