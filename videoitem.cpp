#include <QFileInfo>
#include "videoitem.h"
#include "common.h"

VideoItem::VideoItem(QObject *parent) : QObject(parent)
{
    m_videoName = "";
    m_videoPath = "";
}

VideoItem::VideoItem(Parameters *parameters, QString videoType)
{
    m_parameters = parameters;
    m_videoType = videoType;
}

void VideoItem::serialize(PrettyWriter<StringBuffer> &writer)
{
    writer.StartObject();

    writer.Key("videoType");
    writer.String(m_videoType.toStdString().c_str());

    writer.Key("videoName");
    writer.String(m_videoName.toStdString().c_str());

    writer.Key("videoPath");
    writer.String(m_videoPath.toStdString().c_str());

    writer.EndObject();
}

void VideoItem::unserialize(const Value &value)
{

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
        QFile::copy(tmp.toLocalFile(), m_parameters->getApplicationDirPath().toString() + "/" + VIDEO_FOLDER + "/" + fileInfo.fileName());
        m_videoName = fileInfo.fileName();
    }
    emit videoItemChanged();
}

QString VideoItem::videoPath() const
{
    return m_parameters->getApplicationDirPath().toString() + "/" + VIDEO_FOLDER + "/" + m_videoName;
}

void VideoItem::setVideoPath(const QString &videoPath)
{
    m_videoPath = videoPath;
    emit videoItemChanged();
}
