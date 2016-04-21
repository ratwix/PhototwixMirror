#ifndef VIDEOITEM_H
#define VIDEOITEM_H

#include <QObject>
#include <QQmlContext>
#include <QtQml>
#include "rapidjson/document.h"

#include "parameters.h"
#include "common.h"

using namespace rapidjson;

class VideoItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString videoName READ videoName WRITE setVideoName NOTIFY videoItemChanged)
    Q_PROPERTY(QString videoPath READ videoPath WRITE setVideoPath NOTIFY videoItemChanged)
public:
    explicit VideoItem(QObject *parent = 0);
    VideoItem(Parameters *parameters, VideoType type);

    void serialize(PrettyWriter<StringBuffer> &writer);
    void unserialize(Value const &value);

    QString videoName() const;
    void setVideoName(const QString &videoName);

    QString videoPath() const;
    void setVideoPath(const QString &videoPath);
private:
    QString     m_videoName;
    QString     m_videoPath;
    VideoType   m_videoType;
    QUrl        m_applicationDirPath;
    Parameters  *m_parameters;
signals:
    void    videoItemChanged();
public slots:
};

#endif // VIDEOITEM_H
