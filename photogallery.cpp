#include <dirent.h>
#include <iostream>
#include <fstream>
#include <QQmlEngine>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>
#include "rapidjson/document.h"
#include <sys/stat.h>
#include "photogallery.h"
#include "parameters.h"
#include "common.h"
#include "filedownloader.h"

#define PHOTO_DEST  "photos"

PhotoGallery::PhotoGallery()
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

PhotoGallery::PhotoGallery(Parameters *parameter)
{
    m_parameter = parameter;
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

PhotoGallery::~PhotoGallery()
{
    //Free photos
    CLog::Write(CLog::Debug, "Delete PhotoGallery");


    QList<QObject*>::iterator it;

    for (it = m_photoList.begin(); it != m_photoList.end(); it++) {
        delete *it; //TODO test delete OK
    }
}

Photo* PhotoGallery::addPhoto(QString name, Template *t)
{
    Photo *p = new Photo(name, t);
    QQmlEngine::setObjectOwnership(p, QQmlEngine::CppOwnership);
    m_photoList.prepend(p);
    emit photoListChanged();
    return p;
}

/**
 * @brief PhotoGallery::addPhoto Ajoute une photo depuis tweeter
 * @return La nouvelle photo crée, vide de ses images
 */
Photo *PhotoGallery::addPhotoTwitter(QString name, Template *t, QString effectName, QString twitterMessage, QString twitterProfileName, QUrl twitterPhotoSource)
{
    //Download and creasete the photo
    CLog::Write(CLog::Debug, "Add a new twitter Photo");

    Photo *p = new Photo(name, t);
    QQmlEngine::setObjectOwnership(p, QQmlEngine::CppOwnership);
    p->setPhotoTweeter(true);
    p->setPhotoTweeterMessage(twitterMessage);
    p->setPhotoTweeterProfileName(twitterProfileName);
    p->setEffectName(effectName);

    //Download image from URL and attach it to first gallery item
    CLog::Write(CLog::Debug, QString("Try to download ") + twitterPhotoSource.toDisplayString() + " to folder" + QString(PHOTOSS_FOLDER));
    FileDownloader *imageDownloader = new FileDownloader(twitterPhotoSource, PHOTOSS_FOLDER, p, m_applicationDirPath, 0, this);
    QQmlEngine::setObjectOwnership(imageDownloader, QQmlEngine::CppOwnership);

    QObject::connect(imageDownloader, &FileDownloader::downloaded, this, &PhotoGallery::imageDownloaded);

    QQmlEngine::setObjectOwnership(p, QQmlEngine::CppOwnership);
    return p;
}

Photo *PhotoGallery::addPhotoMirror(QString name, Template *t, QString effectName, QString mirrorIP, QString photoPath, QList<QString> photosList)
{
    //TODO: attention a photoList car l'objet qui le contient est deleted. Passer en pointer ?
    CLog::Write(CLog::Debug, "Add a new mirror Photo");

    Photo *p = new Photo(name, t);
    QQmlEngine::setObjectOwnership(p, QQmlEngine::CppOwnership);
    p->setPhotoTweeter(false);
    p->setEffectName(effectName);

    //Download image from URL and attach it to first gallery item
    CLog::Write(CLog::Debug, QString("Try to download all photos:") + photosList.length());
    /* Lancer tout les download en même temps --> non car pas de garanti de la liste
     * On sauvegarde la liste actuelle.
     * A chaque réception, supprimer de la liste
     * Si la liste est vide, alors Show Photo
    FileDownloader *imageDownloader = new FileDownloader(twitterPhotoSource, PHOTOSS_FOLDER, p, m_applicationDirPath, 0, this);

    QQmlEngine::setObjectOwnership(imageDownloader, QQmlEngine::CppOwnership);

    QObject::connect(imageDownloader, &FileDownloader::downloaded, this, &PhotoGallery::imageDownloaded);

    QQmlEngine::setObjectOwnership(p, QQmlEngine::CppOwnership);
    */
    return p;
}

void PhotoGallery::prependPhoto(Photo *photo)
{
    m_photoList.prepend(photo);
    Serialize();
        //Message pour mettre a jour la gallerie
    emit photoListChanged();
    //Unlock pop : image is downloaded and saved
    m_parameter->getPhotoQueueManager()->setCanPop(true);
}

/**
 * Image is downloaded send showPhoto. Delete file downloader
 * @brief PhotoGallery::imageDownloaded
 */
void PhotoGallery::imageDownloaded(Photo *photo) {
    CLog::Write(CLog::Debug, "Image has been downladed");

    //m_photoList.prepend(photo); //Pourquoi on l'ajoute ici alors qu'il y a pas encore le nom ?
    //QObject* downloader = sender();
    //delete downloader; //TODO test delete : BAD GUY, not good delete ???
    CLog::Write(CLog::Debug, "Envoie signal pour affichage de la photo");
    //Message pour afficher le résultat
    emit showPhoto(photo);
}

void PhotoGallery::removePhoto(QString name)
{
    QList<QObject*>::iterator it;

    for (it = m_photoList.begin(); it != m_photoList.end(); it++) {
        if (Photo *p = dynamic_cast<Photo*>(*it)) {
            if (name == p->name()) {
                m_photoList.removeOne(p);

                //Move photo to "deleted" directory
                string ip = string(PHOTOS_PATH);
                string dest = p->finalResult().toString().replace(0, ip.length(), QString(PHOTOSD_PATH)).toStdString();
                rename(p->finalResult().toString().toStdString().c_str(),
                       dest.c_str());
                //Move all photo part
                for (QList<QObject*>::const_iterator it2 = p->photoPartList().begin(); it2 != p->photoPartList().end(); it2++) {
                    if (PhotoPart const *pp = dynamic_cast<PhotoPart*>(*it2)) {
                        string ip2 = string(PHOTOSS_PATH);
                        string dest = pp->path().toString().replace(0, ip2.length(), QString(PHOTOSDS_PATH)).toStdString();
                        rename(pp->path().toString().toStdString().c_str(),
                               dest.c_str());
                    }
                }
                emit photoListChanged();
                //delete p; //TODO: memory leak but crash in QML
                break;
            }
        }
    }
    Serialize();
}

static qint64 getDirectoryFileSize(QString s, int &nb) {
    qint64 totalSizeByte = 0;

    QDir photoDir(s);
    if (photoDir.exists()) {
         QFileInfoList list = photoDir.entryInfoList();
         for (int i = 0; i < list.size(); ++i) {
             QFileInfo fileInfo = list.at(i);
             if (!fileInfo.isDir()) {
                 totalSizeByte += fileInfo.size();
                 nb += 1;
             }
         }
     }

    return totalSizeByte;
}

void PhotoGallery::updateGalleryDiskSize(bool saveSingle, bool saveDeleted, bool saveDeletedSingle) {
    qint64 totalSizeByte = 0;
    int    nb;
    totalSizeByte += getDirectoryFileSize(PHOTOS_PATH, nb);

    if (saveSingle) {
        totalSizeByte += getDirectoryFileSize(PHOTOSS_PATH, nb);
    }

    if (saveDeleted) {
        totalSizeByte += getDirectoryFileSize(PHOTOSD_PATH, nb);
    }

    if (saveDeletedSingle) {
        totalSizeByte += getDirectoryFileSize(PHOTOSDS_PATH, nb);
    }

    setTotalFileNumber(nb);
    setTotalFileSize(totalSizeByte);
}


void PhotoGallery::saveGallery(QUrl destPath, bool saveSingle, bool saveDeleted, bool saveDeletedSingle)
{
    QtConcurrent::run(this, &PhotoGallery::saveGalleryThread, destPath, saveSingle, saveDeleted, saveDeletedSingle);
    //saveGalleryThread(destPath, saveSingle, saveDeleted, saveDeletedSingle);
}

void PhotoGallery::saveGalleryThread(QUrl d, bool saveSingle, bool saveDeleted, bool saveDeletedSingle)
{
    emit copyStart();

    int current = 0;
    setCurrentCopy(current);

    updateGalleryDiskSize(saveSingle, saveDeleted, saveDeletedSingle);

    QString destPath = d.toLocalFile();
    /* toLocalFile() remove file:///
    string prefix("file://");

    if (destPath.toStdString().substr(0, prefix.size()) == prefix) {
        destPath = QString(destPath.toStdString().substr(prefix.size(), destPath.size() - prefix.size()).c_str());
    }
    */
    if (destPath.toStdString().back() != '/') {
        destPath = destPath + "/";
    }

    QList<QObject*>::iterator it;

    //Copy all photos from the gallery --> basic photos

    for (it = m_photoList.begin(); it != m_photoList.end(); it++) {
        if (Photo *p = dynamic_cast<Photo*>(*it)) {
            QFileInfo f(p->finalResult().toDisplayString(QUrl::PreferLocalFile | QUrl::NormalizePathSegments));
            QFile file(p->finalResult().toDisplayString(QUrl::PreferLocalFile | QUrl::NormalizePathSegments));

            if (!f.exists()) {
                CLog::Write(CLog::Debug, "COPY : File do not exist" + p->finalResult().toDisplayString(QUrl::PreferLocalFile | QUrl::NormalizePathSegments).toStdString());
                continue ;
            }

            //Create a directory to contain photos
            QDir d;
            d.mkpath(destPath + PHOTO_DEST);
            CLog::Write(CLog::Debug, (destPath + "/" + PHOTO_DEST).toStdString());
            file.copy(destPath + PHOTO_DEST + "/" + f.fileName());

            CLog::Write(CLog::Debug, ("COPY : File " + destPath + PHOTO_DEST + "/" + f.fileName()).toStdString());
            setCurrentCopy(++current);
        }
    }

    //Save single files
    if (saveSingle) {
        QDir singleDir(PHOTOSS_PATH);

        if (singleDir.exists()) {
            QDir d;
            d.mkpath(destPath + PHOTO_DEST + "/single/");

            QFileInfoList list = singleDir.entryInfoList();
            for (int i = 0; i < list.size(); ++i) {
                QFileInfo fileInfo = list.at(i);

                if (!fileInfo.isDir()) {
                    QFile::copy(fileInfo.absoluteFilePath(), destPath + PHOTO_DEST + "/single/" + fileInfo.fileName());
                    setCurrentCopy(++current);
                }
            }
        }
    }

    //Save deleted files
    if (saveDeleted) {
        QDir delDir(PHOTOSD_PATH);

        if (delDir.exists()) {
            QDir d;
            d.mkpath(destPath + PHOTO_DEST + "/deleted/");

            QFileInfoList list = delDir.entryInfoList();
            for (int i = 0; i < list.size(); ++i) {
                QFileInfo fileInfo = list.at(i);

                if (!fileInfo.isDir()) {
                    QFile::copy(fileInfo.absoluteFilePath(), destPath + PHOTO_DEST + "/deleted/" + fileInfo.fileName());
                    setCurrentCopy(++current);
                }
            }
        }
    }

    //Save deleted single
    if (saveDeletedSingle) {
        QDir delDirSingle(PHOTOSDS_PATH);

        if (delDirSingle.exists()) {
            QDir d;
            d.mkpath(destPath + PHOTO_DEST + "/deleted/single");

            QFileInfoList list = delDirSingle.entryInfoList();
            for (int i = 0; i < list.size(); ++i) {
                QFileInfo fileInfo = list.at(i);

                if (!fileInfo.isDir()) {
                    QFile::copy(fileInfo.absoluteFilePath(), destPath + PHOTO_DEST + "/deleted/single/" + fileInfo.fileName());
                    setCurrentCopy(++current);
                }
            }
        }
    }

    emit copyEnd();
}

void PhotoGallery::addPhoto(const Value &value, QList<QObject*> &templates)
{
    //Find the right template
    if (value.HasMember("currentTemplate")) {
        QString tmp = QString(value["currentTemplate"].GetString());

        Template *tmp_dest = 0;
        for (QList<QObject*>::const_iterator it = templates.begin(); it != templates.end(); it++) {
            if (Template *t = dynamic_cast<Template*>(*it)) {
                if (tmp == t->getName()) {
                    tmp_dest = t;
                    break;
                }
            }
        }

        if (tmp_dest != 0) {
            Photo *photo = new Photo(value, tmp_dest);
            QQmlEngine::setObjectOwnership(photo, QQmlEngine::CppOwnership);
            m_photoList.append(photo);
        } else {
            CLog::Write(CLog::Debug, "No dest template found");
        }

    } else {
        CLog::Write(CLog::Debug, "No current template");
    }
}



int PhotoGallery::totalFileNumber() const
{
    return m_totalFileNumber;
}

void PhotoGallery::setTotalFileNumber(int totalFileNumber)
{
    m_totalFileNumber = totalFileNumber;
    emit totalFileNumberChanged();
}

qint64 PhotoGallery::totalFileSize() const
{
    return m_totalFileSize;
}

void PhotoGallery::setTotalFileSize(const qint64 &totalFileSize)
{
    m_totalFileSize = totalFileSize;
    emit totalFileSizeChanged();
}

int PhotoGallery::currentCopy() const
{
    return m_currentCopy;
}

void PhotoGallery::setCurrentCopy(int currentCopy)
{
    m_currentCopy = currentCopy;
    emit copyProgress(m_currentCopy, m_totalFileNumber);
    emit currentCopyChanged();
}


void PhotoGallery::Serialize()
{
    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);

    CLog::Write(CLog::Debug, "Serialize Gallery");

    writer.StartObject();
    writer.Key("photos");
    writer.StartArray();
    for (QList<QObject*>::const_iterator it = m_photoList.begin(); it != m_photoList.end(); it++) {
        if (Photo *p = dynamic_cast<Photo*>(*it)) {
            p->Serialize(writer);
        } else {
            CLog::Write(CLog::Fatal, "Bad type QObject -> Photo");
        }
    }
    writer.EndArray();
    writer.EndObject();

    //write to file
    std::ofstream jsonFile;
    jsonFile.open(GALLERY_FILE);
    jsonFile << sb.GetString();

    if (!jsonFile.good()) {
        CLog::Write(CLog::Fatal, "Can't write the JSON string to the gallery file!");
    } else {
        CLog::Write(CLog::Debug, "Write the JSON string to the gallery file!");
    }

    jsonFile.close();
}

void PhotoGallery::Unserialize(QList<QObject*> &templates)
{
    ifstream jsonFile(GALLERY_FILE, ios::in);

    if (!jsonFile) {
        CLog::Write(CLog::Info, "JSON gallery File not exist");
        return;
    }

    std::string str;

    jsonFile.seekg(0, std::ios::end);
    str.reserve(jsonFile.tellg());
    jsonFile.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(jsonFile)),
                std::istreambuf_iterator<char>());

    //CLog::Write(CLog::Debug, "Fichier json gallery : " + str);

    Document document;
    document.Parse(str.c_str());
    jsonFile.close();


    if (document.HasMember("photos")) {
        const Value& photos = document["photos"];
        if (photos.IsArray()) {
            for (SizeType i = 0; i < photos.Size(); i++) {
                addPhoto(photos[i], templates);
            }
        }
    }

    emit photoListChanged();
}


QList<QObject *> PhotoGallery::photoList() const
{
    CLog::Write(CLog::Debug, "Get Photo list " + itos(m_photoList.length()));
    return m_photoList;
}

void PhotoGallery::setPhotoList(const QList<QObject *> &photoList)
{
    m_photoList = photoList;
    emit photoListChanged();
}
QUrl PhotoGallery::applicationDirPath() const
{
    return m_applicationDirPath;
}

void PhotoGallery::setApplicationDirPath(const QUrl &applicationDirPath)
{
    m_applicationDirPath = applicationDirPath;
}



