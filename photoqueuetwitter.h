#ifndef PHOTOQUEUETWITTER_H
#define PHOTOQUEUETWITTER_H

#include <QObject>
#include "photoqueue.h"

class PhotoQueueTwitter : public PhotoQueue
{
public:
    PhotoQueueTwitter();
    virtual ~PhotoQueueTwitter();

    QString twitId() const;
    void setTwitId(const QString &twitId);

    QString twitText() const;
    void setTwitText(const QString &twitText);

    QString twitProfileName() const;
    void setTwitProfileName(const QString &twitProfileName);

    QString twitProfileImage() const;
    void setTwitProfileImage(const QString &twitProfileImage);

    QUrl twitMediaUrl() const;
    void setTwitMediaUrl(const QUrl &twitMediaUrl);

private:
    QString     m_twitId;
    QString     m_twitText;
    QString     m_twitProfileName;
    QString     m_twitProfileImage;
    QUrl        m_twitMediaUrl;

signals:

public slots:
};

#endif // PHOTOQUEUETWITTER_H
