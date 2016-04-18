#include <QFileInfo>
#include "videoitem.h"
#include "common.h"

VideoItem::VideoItem(QObject *parent) : QObject(parent)
{
    m_videoName = "";
    m_videoPath = "";
}

VideoItem::VideoItem(QUrl appDirPath)
{
    m_applicationDirPath = appDirPath;
}

QString VideoItem::videoName() const
{
    return m_videoName;
}

void VideoItem::setVideoName(const QString &videoName)
{
    QUrl tmp(videoName);


    QFileInfo fileInfo(tmp.toLocalFile());

    if (fileInfo.exists()) {
        //Copy video to video path
        QFile::copy(tmp.toLocalFile(), m_applicationDirPath.toString() + "/" + VIDEO_FOLDER + "/" + fileInfo.fileName());
        m_videoName = fileInfo.fileName();
    }
    emit videoItemChanged();
}

QString VideoItem::videoPath() const
{
    return m_applicationDirPath.toString() + "/" + VIDEO_FOLDER + "/" + m_videoName;
}

void VideoItem::setVideoPath(const QString &videoPath)
{
    m_videoPath = videoPath;
    emit videoItemChanged();
}
