#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "photogallery.h"

class FileDownloader : public QObject
{
 Q_OBJECT
 public:
  explicit FileDownloader(QUrl imageUrl, QString distDir, Photo *photo, QUrl applicationDirPath, QObject *parent);
  virtual ~FileDownloader();
  QByteArray downloadedData() const;

  QUrl applicationDirPath() const;
  void setApplicationDirPath(const QUrl &applicationDirPath);

signals:
  void downloaded(Photo *p);

private slots:
  void fileDownloaded(QNetworkReply* pReply);

private:
  QNetworkAccessManager m_webCtrl;
  QByteArray m_downloadedData;
  QString   m_distDir;
  Photo *   m_photo;
  QUrl      m_url;
  QUrl      m_applicationDirPath;
};

#endif // FILEDOWNLOADER_H
