#include "parameters.h"

Parameters::Parameters(QUrl appDirPath)
{
    m_applicationDirPath = appDirPath;
    init();
}

void Parameters::init()
{
    m_waitVideo = new VideoItem(m_applicationDirPath);
    m_startGlobalPhotoProcessVideo = new VideoItem(m_applicationDirPath);
    m_startPhotoProcessVideo = new VideoItem(m_applicationDirPath);
    m_endGlobalPhotoProcessVideo = new VideoItem(m_applicationDirPath);
}

VideoItem *Parameters::endGlobalPhotoProcessVideo() const
{
    return m_endGlobalPhotoProcessVideo;
}

void Parameters::setEndGlobalPhotoProcessVideo(VideoItem *endGlobalPhotoProcessVideo)
{
    m_endGlobalPhotoProcessVideo = endGlobalPhotoProcessVideo;
}

VideoItem *Parameters::startGlobalPhotoProcessVideo() const
{
    return m_startGlobalPhotoProcessVideo;
}

void Parameters::setStartGlobalPhotoProcessVideo(VideoItem *startGlobalPhotoProcessVideo)
{
    m_startGlobalPhotoProcessVideo = startGlobalPhotoProcessVideo;
}

VideoItem *Parameters::startPhotoProcessVideo() const
{
    return m_startPhotoProcessVideo;
}

void Parameters::setStartPhotoProcessVideo(VideoItem *startPhotoProcessVideo)
{
    m_startPhotoProcessVideo = startPhotoProcessVideo;
}

VideoItem *Parameters::waitVideo() const
{
    return m_waitVideo;
}

void Parameters::setWaitVideo(VideoItem *waitVideo)
{
    m_waitVideo = waitVideo;
}
