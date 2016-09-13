#include "filedownloader.h"

FileDownloader::FileDownloader(QUrl imageUrl, QString distPath, Photo *photo, QUrl applicationDirPath, int imageNumber, QObject *parent) :
 QObject(parent)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    m_applicationDirPath = applicationDirPath;
    m_distDir = distPath;
    m_photo = photo;
    m_url = imageUrl;
    m_imageNumber = imageNumber;
    connect(
        &m_webCtrl,
                SIGNAL (finished(QNetworkReply*)),
                this,
                SLOT (fileDownloaded(QNetworkReply*))
    );

    CLog::Write(CLog::Debug, QString("Démarage du download") + imageUrl.toDisplayString());
    QNetworkRequest request(imageUrl);
    m_webCtrl.get(request);
}

FileDownloader::~FileDownloader() {
    CLog::Write(CLog::Debug, "Delete FileDownloader");
}

/**
 * @brief FileDownloader::fileDownloaded
 * @param pReply
 * Quand l'image a fini d'être uploader, on la sauvegarde, on l'ajoute a la photo actuelle, et on serialise
 */

void FileDownloader::fileDownloaded(QNetworkReply* pReply) {
    CLog::Write(CLog::Debug, "Fin du download de l'image");
    m_downloadedData = pReply->readAll();
    CLog::Write(CLog::Debug, "Sauvegarde image tweeter dans " + m_applicationDirPath.toDisplayString() + "/" + m_distDir + "/" + m_url.fileName());
    QFile file(m_applicationDirPath.toDisplayString() + "/" + m_distDir + "/" + m_url.fileName());
    if (file.open(QIODevice::WriteOnly)) {
        file.write(m_downloadedData);
        file.close();
        CLog::Write(CLog::Debug, "Fin de l'ecriture du download de l'image");
        if (m_photo->photoPartList().length() > m_imageNumber) {
            if (PhotoPart *photoPart = dynamic_cast<PhotoPart *>(m_photo->photoPartList().at(m_imageNumber))) { //TODO: changer 0 avec un parametre a mettre dans le constructeur
                QFileInfo fi(file);
                QUrl destPath(QString("file:///") + m_applicationDirPath.toDisplayString() + "/" + m_distDir + "/" + fi.fileName());
                photoPart->setPath(destPath);
                emit m_photo->photoPartListChange();
            } else {
                CLog::Write(CLog::Warning, "[Warning]Pas assez de partie pour l'image tweeter");
            }
        }
    } else {
        CLog::Write(CLog::Warning, "[Warning]Echec d'ouverture de l'image tweeter");
    }
    //emit a signal
    pReply->deleteLater();
    //emit m_photo->tweeterLoadingComplete();
    emit downloaded(m_photo);
}

QUrl FileDownloader::applicationDirPath() const
{
    return m_applicationDirPath;
}

void FileDownloader::setApplicationDirPath(const QUrl &applicationDirPath)
{
    m_applicationDirPath = applicationDirPath;
}

QByteArray FileDownloader::downloadedData() const {
    return m_downloadedData;
}
