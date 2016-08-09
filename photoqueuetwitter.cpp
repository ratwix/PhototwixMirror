#include "photoqueue.h"
#include "photoqueuetwitter.h"


PhotoQueueTwitter::PhotoQueueTwitter()
{

}

PhotoQueueTwitter::~PhotoQueueTwitter()
{

}

QString PhotoQueueTwitter::twitId() const
{
    return m_twitId;
}

void PhotoQueueTwitter::setTwitId(const QString &twitId)
{
    m_twitId = twitId;
}

QString PhotoQueueTwitter::twitText() const
{
    return m_twitText;
}

void PhotoQueueTwitter::setTwitText(const QString &twitText)
{
    m_twitText = twitText;
}

QString PhotoQueueTwitter::twitProfileName() const
{
    return m_twitProfileName;
}

void PhotoQueueTwitter::setTwitProfileName(const QString &twitProfileName)
{
    m_twitProfileName = twitProfileName;
}

QString PhotoQueueTwitter::twitProfileImage() const
{
    return m_twitProfileImage;
}

void PhotoQueueTwitter::setTwitProfileImage(const QString &twitProfileImage)
{
    m_twitProfileImage = twitProfileImage;
}

QUrl PhotoQueueTwitter::twitMediaUrl() const
{
    return m_twitMediaUrl;
}

void PhotoQueueTwitter::setTwitMediaUrl(const QUrl &twitMediaUrl)
{
    m_twitMediaUrl = twitMediaUrl;
}
