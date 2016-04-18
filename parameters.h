#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <QObject>
#include "videoitem.h"
class Parameters : public QObject
{
    Q_OBJECT
    Q_PROPERTY(VideoItem* waitVideo READ waitVideo WRITE setWaitVideo NOTIFY parameterVideoChanged)
    Q_PROPERTY(VideoItem* startGlobalPhotoProcessVideo READ startGlobalPhotoProcessVideo WRITE setStartGlobalPhotoProcessVideo NOTIFY parameterVideoChanged)
    Q_PROPERTY(VideoItem* endGlobalPhotoProcessVideo READ endGlobalPhotoProcessVideo WRITE setEndGlobalPhotoProcessVideo NOTIFY parameterVideoChanged)
    Q_PROPERTY(VideoItem* startPhotoProcessVideo READ startPhotoProcessVideo WRITE setStartPhotoProcessVideo NOTIFY parameterVideoChanged)
public:
    Parameters(QUrl appDirPath);

    VideoItem *waitVideo() const;
    void setWaitVideo(VideoItem *waitVideo);

    VideoItem *startPhotoProcessVideo() const;
    void setStartPhotoProcessVideo(VideoItem *startPhotoProcessVideo);

    VideoItem *startGlobalPhotoProcessVideo() const;
    void setStartGlobalPhotoProcessVideo(VideoItem *startGlobalPhotoProcessVideo);

    VideoItem *endGlobalPhotoProcessVideo() const;
    void setEndGlobalPhotoProcessVideo(VideoItem *endGlobalPhotoProcessVideo);

private:
    void init();

private:
    QUrl      m_applicationDirPath;

    VideoItem *m_waitVideo;
    VideoItem *m_startGlobalPhotoProcessVideo;
    VideoItem *m_startPhotoProcessVideo;
    VideoItem *m_endGlobalPhotoProcessVideo;

signals:
    void parameterVideoChanged();

};

#endif // PARAMETERS_H
