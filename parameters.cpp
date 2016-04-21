#include "parameters.h"

#include <QQmlEngine>
#include <QDir>
#include <QtConcurrent>
#include <QFile>
#include <iostream>
#include <fstream>

#include "rapidjson/document.h"
#include "clog.h"

using namespace std;
using namespace rapidjson;

/**
 * Static useful functions
 */

static std::string pexec(const char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}

static bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

/**
 * Constructor / Destructor / init
 */

Parameters::Parameters(QUrl appDirPath)
{
    m_applicationDirPath = appDirPath;
    init();
}

Parameters::~Parameters()
{
    //Free templates
    QList<QObject*>::iterator it;

    for (it = m_templates.begin(); it != m_templates.end(); it++) {
        delete *it;
    }
}

void Parameters::init() {
    m_nbprint = 0;
    m_nbfreephotos = 250;
    m_pricephoto = 0.4;
    m_flipcamera = false;
    m_flipresult = false;
    m_backgroundImage = "";
    m_cameraHight = 1728;
    m_cameraWidth = 2592;
    m_blockPrint = false;
    m_blockPrintNb = 700;
    m_paperprint = 0;
    createFolders();
    m_photogallery = new PhotoGallery();
    m_photogallery->setApplicationDirPath(m_applicationDirPath);
    m_mailActive = false;
    m_mailFrom = "";
    m_mailCc = "";
    m_mailBcc = "";
    m_mailSmtp = "";
    m_mailPort = "";
    m_mailUsername = "";
    m_mailPassword = "";
    m_mailSubject = "";
    m_mailContent = "";

    m_waitVideo = new VideoItem(this, VIDEO_WAIT);
    m_startGlobalPhotoProcessVideo = new VideoItem(this, VIDEO_STARTGLOBALPHOTOPROCESS);
    m_startPhotoProcessVideo = new VideoItem(this, VIDEO_STARTPHOTOPROCESS);
    m_endGlobalPhotoProcessVideo = new VideoItem(this, VIDEO_ENDGLOBALPHOTOPROCESS);

    Unserialize(); //Unserialize parameters
    m_photogallery->Unserialize(m_templates); //unserialize current gallery

    //Read all .png and .jpg files in tempalte directory
    readTemplateDir();
    updatePaperPrint();
    Serialize();
    rebuildActivesTemplates();
}

/**
  * Serialization
  */

void Parameters::Serialize() {
    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);

    writer.StartObject();
    //save standard elements

        //save template definition
        writer.Key("nbPrint");
        writer.Int(m_nbprint);

        writer.Key("nbFreePhoto");
        writer.Int(m_nbfreephotos);

        writer.Key("pricephoto");
        writer.Double(m_pricephoto);

        writer.Key("flipcamera");
        writer.Bool(m_flipcamera);

        writer.Key("flipresult");
        writer.Bool(m_flipresult);

        writer.Key("cameraHeight");
        writer.Int(m_cameraHight);

        writer.Key("cameraWidth");
        writer.Int(m_cameraWidth);

        writer.Key("blockPrint");
        writer.Bool(m_blockPrint);

        writer.Key("blockPrintNb");
        writer.Int(m_blockPrintNb);

        writer.Key("paperprint");
        writer.Int(m_paperprint);

        writer.Key("backgroundImage");
        writer.String(m_backgroundImage.toStdString().c_str());

        writer.Key("mailActive");
        writer.Bool(m_mailActive);

        writer.Key("mailFrom");
        writer.String(m_mailFrom.toStdString().c_str());

        writer.Key("mailCc");
        writer.String(m_mailCc.toStdString().c_str());

        writer.Key("mailBcc");
        writer.String(m_mailBcc.toStdString().c_str());

        writer.Key("mailSmtp");
        writer.String(m_mailSmtp.toStdString().c_str());

        writer.Key("mailPort");
        writer.String(m_mailPort.toStdString().c_str());

        writer.Key("mailUsername");
        writer.String(m_mailUsername.toStdString().c_str());

        writer.Key("mailPassword");
        writer.String(m_mailPassword.toStdString().c_str());

        writer.Key("mailSubject");
        writer.String(m_mailSubject.toStdString().c_str());

        writer.Key("mailContent");
        writer.String(m_mailContent.toStdString().c_str());

        writer.Key("templates");
        writer.StartArray();
        for (QList<QObject*>::const_iterator it = m_templates.begin(); it != m_templates.end(); it++) {
            if (Template *t = dynamic_cast<Template*>(*it)) {
                t->Serialize(writer);
            } else {
                CLog::Write(CLog::Fatal, "Bad type QObject -> Template");
            }
        }
        writer.EndArray();

        writer.Key("videos");
        writer.StartArray();
        m_waitVideo->serialize(writer);
        m_startGlobalPhotoProcessVideo->serialize(writer);
        m_startPhotoProcessVideo->serialize(writer);
        m_endGlobalPhotoProcessVideo->serialize(writer);
        writer.EndArray();

    writer.EndObject();

    //write to file
    std::ofstream jsonFile;

    jsonFile.open(CONFIG_FILE);
    jsonFile << sb.GetString();

    if (!jsonFile.good()) {
        CLog::Write(CLog::Fatal, "Can't write the JSON string to the file!");
    } else {
        CLog::Write(CLog::Debug, "Write the JSON string to the file!");
    }

    jsonFile.close();
}

void Parameters::Unserialize() {
    ifstream jsonFile(CONFIG_FILE, ios::in);

    if (!jsonFile) {
        CLog::Write(CLog::Info, "JSON File not exist");
        return;
    }

    std::string str;

    jsonFile.seekg(0, std::ios::end);
    str.reserve(jsonFile.tellg());
    jsonFile.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(jsonFile)),
                std::istreambuf_iterator<char>());

    Document document;
    document.Parse(str.c_str());

    jsonFile.close();

    if (document.HasMember("nbPrint")) {
        m_nbprint = document["nbPrint"].GetInt();
    }

    if (document.HasMember("nbFreePhoto")) {
        m_nbfreephotos = document["nbFreePhoto"].GetInt();
    }

    if (document.HasMember("pricephoto")) {
        m_pricephoto = document["pricephoto"].GetDouble();
    }

    if (document.HasMember("flipcamera")) {
        m_flipcamera = document["flipcamera"].GetBool();
    }

    if (document.HasMember("flipresult")) {
       m_flipresult = document["flipresult"].GetBool();
    }

    if (document.HasMember("backgroundImage")) {
       m_backgroundImage = QString(document["backgroundImage"].GetString());
    }

    if (document.HasMember("cameraHeight")) {
       m_cameraHight = document["cameraHeight"].GetInt();
    }

    if (document.HasMember("cameraWidth")) {
       m_cameraWidth = document["cameraWidth"].GetInt();
    }

    if (document.HasMember("blockPrint")) {
       m_blockPrint = document["blockPrint"].GetBool();
    }

    if (document.HasMember("blockPrintNb")) {
       m_blockPrintNb = document["blockPrintNb"].GetInt();
    }

    if (document.HasMember("paperprint")) {
        m_paperprint = document["paperprint"].GetInt();
    }

    if (document.HasMember("mailActive")) {
        m_mailActive = document["mailActive"].GetBool();
    }

    if (document.HasMember("mailFrom")) {
        m_mailFrom = QString(document["mailFrom"].GetString());
    }

    if (document.HasMember("mailCc")) {
        m_mailCc = QString(document["mailCc"].GetString());
    }

    if (document.HasMember("mailBcc")) {
        m_mailBcc = QString(document["mailBcc"].GetString());
    }

    if (document.HasMember("mailSmtp")) {
        m_mailSmtp = QString(document["mailSmtp"].GetString());
    }

    if (document.HasMember("mailPort")) {
        m_mailPort = QString(document["mailPort"].GetString());
    }

    if (document.HasMember("mailUsername")) {
        m_mailUsername = QString(document["mailUsername"].GetString());
    }

    if (document.HasMember("mailPassword")) {
        m_mailPassword = QString(document["mailPassword"].GetString());
    }

    if (document.HasMember("mailSubject")) {
        m_mailSubject = QString(document["mailSubject"].GetString());
    }

    if (document.HasMember("mailContent")) {
        m_mailContent = QString(document["mailContent"].GetString());
    }

    if (document.HasMember("templates")) {
        const Value& templates = document["templates"];
        if (templates.IsArray()) {
            for (SizeType i = 0; i < templates.Size(); i++) {
                addTemplate(templates[i]);
            }
        }
    }

    if (document.HasMember("videos")) {
        const Value& videos = document["videos"];
        if (videos.IsArray()) {
            for (SizeType i = 0; i < videos.Size(); i++) {
                if (videos[i].HasMember("videoType")) {
                    VideoType type = (VideoType)videos[i]["videoType"].GetInt();
                    switch (type) {
                        case VIDEO_WAIT:
                            m_waitVideo->unserialize(videos[i]);
                        break;
                        case VIDEO_STARTGLOBALPHOTOPROCESS:
                            m_startGlobalPhotoProcessVideo->unserialize(videos[i]);
                        break;
                        case VIDEO_STARTPHOTOPROCESS:
                            m_startPhotoProcessVideo->unserialize(videos[i]);
                        break;
                        case VIDEO_ENDGLOBALPHOTOPROCESS:
                            m_endGlobalPhotoProcessVideo->unserialize(videos[i]);
                        break;
                    default:
                        CLog::Write(CLog::Error, QString("Unknown video type:") + type);
                    }
                }
            }
        }
    }
}


/**
 * Parameter functions
 */

void Parameters::createFolders()
{
    QDir d;

    d.mkpath(QString(VIDEO_PATH));
    d.mkpath(QString(PHOTOS_PATH));
    d.mkpath(QString(PHOTOSSD_PATH));
    d.mkpath(QString(PHOTOSS_PATH));
    d.mkpath(QString(PHOTOSD_PATH));
    d.mkpath(QString(PHOTOSDS_PATH));
    d.mkpath(QString(BACKGROUND_PATH));
    d.mkpath(QString(TEMPLATE_PATH));
}

void Parameters::changeBackground(QUrl url) {
    QFileInfo fileInf(url.toLocalFile());
    QString filename = fileInf.fileName();

    //Copy file to local template directory
    if (fileInf.exists()) {
        QFile::copy(fileInf.absoluteFilePath(), QString(BACKGROUND_PATH) + "/" + filename);
        setBackgroundImage("file:///" + QString(BACKGROUND_PATH) + "/" + filename);
    } else {
        setBackgroundImage("");
    }
}

void Parameters::updatePaperPrint()
{
    if (system(NULL)) {
        string result;

        string cmd = m_applicationDirPath.toString().toStdString() + "/print/get_paper.sh";
        CLog::Write(CLog::Debug, "Paper cmd :" + cmd);
        result = pexec(cmd.c_str());
        CLog::Write(CLog::Debug, "Paper result :" + result);
        if (is_number(result)) {
            setPaperprint(atoi(result.c_str()));
        } else {
            setPaperprint(699);
        }
    }
}

void Parameters::haltSystem()
{
    if (system(NULL)) {
        string cmd = m_applicationDirPath.toString().toStdString() + "/scripts/halt.sh";
        pexec(cmd.c_str());
    }
}

/**
 * Print management
 */

void Parameters::printPhoto(QUrl url, bool doubleprint, bool cutprint, bool landscape)
{
    CLog::Write(CLog::Debug, "Print file : " + url.toString().toStdString() + " double:" + (doubleprint ? "true":"false") + " cut:" + (cutprint?"true":"false") + " landscape:" + (landscape?"true":"false"));
    QtConcurrent::run(this, &Parameters::printThread, m_applicationDirPath, url, doubleprint, cutprint, landscape);
    setNbprint(m_nbprint + 1);
}

void Parameters::printThread(QUrl m_applicationDirPath, QUrl url, bool doubleprint, bool cutprint, bool landscape) {
    if (system(NULL)) {
        string cmd = m_applicationDirPath.toString().toStdString() + "/print/print.sh" +
                " duplicate:" + (doubleprint ? "true":"false") +
                " portrait:" + (landscape?"false":"true") +
                " cutter:" + (cutprint?"true":"false") +
                " " + url.toString().toStdString();
        //std::replace(cmd.begin(), cmd.end(), '/', '\\'); //TODO: uncomment on windows
        cmd = cmd;
        CLog::Write(CLog::Debug, "Print cmd :" + cmd);
        system(cmd.c_str());
    }
}

/**
  * Template Management
  */

void Parameters::addTemplate(QString name) {
    bool find = false;
    //Research if template already exist in base, already configured
    for (QList<QObject*>::iterator it = m_templates.begin(); it != m_templates.end(); it++) {
        if (Template *t = dynamic_cast<Template*>(*it)) {
            if (t->getName() == name) {
                find = true;
                CLog::Write(CLog::Debug, "Template " + name.toStdString() + " already configured");
                break;
            }
        }
    }

    if (!find) {
        CLog::Write(CLog::Info, "Add Template " + name.toStdString());
        Template *t = new Template(name, this);
        QQmlEngine::setObjectOwnership(t, QQmlEngine::CppOwnership);
        m_templates.append(t);
    }
}

void Parameters::addTemplate(Value const &value) {
    CLog::Write(CLog::Info, "Load Template ");

    QFileInfo fileInfo(QString((string(TEMPLATE_PATH) + "/" + value["template_name"].GetString()).c_str()));

    if (fileInfo.exists()) {
        Template *t = new Template(value, this);
        QQmlEngine::setObjectOwnership(t, QQmlEngine::CppOwnership);
        QFileInfo fileInf(t->getUrl().toLocalFile());
        if (fileInf.exists()) {
            m_templates.append(t);
        } else {
            delete t;
        }
    } else {
        CLog::Write(CLog::Info, "Template file not found " + string(value["template_name"].GetString()));
    }
}

void Parameters::addTemplateFromUrl(QUrl url)
{
    QFileInfo fileInf(url.toLocalFile());
    QString filename = fileInf.fileName();

    //Copy file to local template directory
    if (fileInf.exists()) {
        QFile::copy(fileInf.absoluteFilePath(), m_applicationDirPath.toString() + "/templates/" + filename);
        Template *t = new Template(filename, this);
        QQmlEngine::setObjectOwnership(t, QQmlEngine::CppOwnership);
        m_templates.append(t);
        emit templatesChanged();
    }
}

void Parameters::deleteTemplateFromName(QString name)
{
    for (QList<QObject*>::const_iterator it = m_templates.begin(); it != m_templates.end(); it++) {
        if (Template *t = dynamic_cast<Template*>(*it)) {
            QString tname = t->getName();
            if (tname == name) {
                //Delete image file
                QString furl = t->getUrl().toString();
                if (furl.startsWith("file://")) {
                    furl = furl.right(furl.length() - QString("file://").length());
                }
                if (!QFile::remove(furl)) {
                    CLog::Write(CLog::Error, "Unable to delete file : " + t->getUrl().toString().toStdString());
                }
                //Delete template from library
                m_templates.removeOne(*it);

                //Serialize
                Serialize();

                //Update interface
                emit templatesChanged();
            }
        }
    }
}

void Parameters::readTemplateDir() {
    QDir dir(QString(TEMPLATE_PATH));

    if (!dir.exists()) {
        CLog::Write(CLog::Fatal, "unable to open" + string(TEMPLATE_PATH) + " failure; terminating");
        return;
    }

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        QString ext = fileInfo.suffix();

        if (ext == "png" || ext == "jpg" || ext == "gif") {
            addTemplate(fileInfo.fileName());
        }
    }

    /*
    DIR *dir;
    struct dirent *file;

    dir = opendir(TEMPLATE_PATH); //"." refers to the current dir
    if (!dir) {
        CLog::Write(CLog::Fatal, string("opendir() ") + TEMPLATE_PATH + " failure; terminating");
        exit(1);
    }

    errno=0;
    while ((file = readdir(dir))) {
        string fn = string(file->d_name);
        string ext = fn.substr(fn.find_last_of(".") + 1);
        if (ext == "png" || ext == "jpg" || ext == "gif") {
            addTemplate(QString(fn.c_str()));
        }
    }
    if (errno){
        CLog::Write(CLog::Fatal, string("readdir() ") + TEMPLATE_PATH + " failure; terminating");
        exit(1);
    }
    closedir(dir);
    */
}

void Parameters::rebuildActivesTemplates()
{
    m_activesTemplates.clear();

    QList<QObject*>::iterator it;

    for (QList<QObject*>::iterator it = m_templates.begin(); it != m_templates.end(); it++) {
        if (Template *t = dynamic_cast<Template*>(*it)) {
            if (t->getActive()) {
                m_activesTemplates.push_back(t);
                emit activeTemplatesChanged();
            }
        }
    }

}

/**
  * Gallery Management
  */


void Parameters::clearGallery()
{
    clearGallery(false);
}

void Parameters::clearGalleryDeleteImages()
{
    clearGallery(true);
}

static void delAllFileInDirectory(const char* p) {
    QString path(p);
    QDir dir(path);
    dir.setNameFilters(QStringList() << "*.*");
    dir.setFilter(QDir::Files);
    foreach(QString dirFile, dir.entryList())
    {
        dir.remove(dirFile);
    }
}

void Parameters::clearGallery(bool del)
{
    delete m_photogallery;
    m_photogallery = new PhotoGallery();
    m_photogallery->setApplicationDirPath(m_applicationDirPath);


    if (del) {
        delAllFileInDirectory(PHOTOS_PATH);
        delAllFileInDirectory(PHOTOSSD_PATH);
        delAllFileInDirectory(PHOTOSS_PATH);
        delAllFileInDirectory(PHOTOSD_PATH);
        delAllFileInDirectory(PHOTOSDS_PATH);
    }

    Serialize();
    m_photogallery->Serialize();
    emit photoGalleryChanged();
}


Photo* Parameters::addPhotoToGallerie(QString name, QObject *temp)
{
    if (name == "") {
        return NULL;
    }
    if (Template *t = dynamic_cast<Template*>(temp)) {
        CLog::Write(CLog::Info, "Creation d'une nouvelle photo " + name.toStdString());
        Photo *p = m_photogallery->addPhoto(name, t);
        emit photoGalleryChanged();
        emit photoGalleryListChanged();
        return p;
    } else {
        CLog::Write(CLog::Warning, "Can't cast template");
    }
    return NULL;
}

/**
 * Accessors
 */

PhotoGallery *Parameters::getPhotogallery() const
{
    return m_photogallery;
}

void Parameters::setPhotogallery(PhotoGallery *photogallery)
{
    m_photogallery = photogallery;
}

QUrl Parameters::getApplicationDirPath() const
{
    return m_applicationDirPath;
}

QList<QObject *> Parameters::getActivesTemplates() const
{
    return m_activesTemplates;
}

QList<QObject*> Parameters::getTemplates() {
    return m_templates;
}

void Parameters::setTemplates(QList<QObject*> templates) {
    this->m_templates = templates;
    emit templatesChanged();
}

void Parameters::setActivesTemplates(const QList<QObject *> &activesTemplates)
{
    m_activesTemplates = activesTemplates;
}

void Parameters::setApplicationDirPath(const QUrl &applicationDirPath)
{
    CLog::Write(CLog::Debug, QUrl("Application dir path " + applicationDirPath.toString()).toString().toStdString());
    m_applicationDirPath = applicationDirPath;
}
int Parameters::getNbprint() const
{
    return m_nbprint;
}

void Parameters::setNbprint(int nbprint)
{
    m_nbprint = nbprint;
    Serialize();
    emit nbPrintChanged();
}
int Parameters::getNbfreephotos() const
{
    return m_nbfreephotos;
}

void Parameters::setNbfreephotos(int nbfreephotos)
{
    m_nbfreephotos = nbfreephotos;
    Serialize();
    emit nbfreephotoChanged();
}
float Parameters::getPricephoto() const
{
    return m_pricephoto;
}

void Parameters::setPricephoto(float pricephoto)
{
    m_pricephoto = pricephoto;
    Serialize();
    emit pricephotoChanged();
}
bool Parameters::getFlipcamera() const
{
    return m_flipcamera;
}

void Parameters::setFlipcamera(bool flipcamera)
{
    m_flipcamera = flipcamera;
    Serialize();
    emit flipcameraChanged();
}
bool Parameters::getFlipresult() const
{
    return m_flipresult;
}

void Parameters::setFlipresult(bool flipresult)
{
    m_flipresult = flipresult;
    Serialize();
    emit flipresultChanged();
}

QString Parameters::getBackgroundImage() const
{
    return m_backgroundImage;
}

void Parameters::setBackgroundImage(const QString &backgroundImage)
{
    m_backgroundImage = backgroundImage;
    Serialize();
    emit backgroundImageChange();
}

int Parameters::getCameraHight() const
{
    return m_cameraHight;
}

void Parameters::setCameraHight(int cameraHight)
{
    m_cameraHight = cameraHight;
    Serialize();
    emit cameraHeightChange();
}

int Parameters::getCameraWidth() const
{
    return m_cameraWidth;
}

void Parameters::setCameraWidth(int cameraWidth)
{
    m_cameraWidth = cameraWidth;
    Serialize();
    emit cameraWidthChange();
}

bool Parameters::getBlockPrint() const
{
    return m_blockPrint;
}

void Parameters::setBlockPrint(bool blockPrint)
{
    m_blockPrint = blockPrint;
    Serialize();
    emit blockPrintChanged();
}
int Parameters::getBlockPrintNb() const
{
    return m_blockPrintNb;
}

void Parameters::setBlockPrintNb(int blockPrintNb)
{
    m_blockPrintNb = blockPrintNb;
    Serialize();
    emit blockPrintNbChanged();
}

int Parameters::paperprint() const
{
    return m_paperprint;
}

void Parameters::setPaperprint(int paperprint)
{
    m_paperprint = paperprint;
    Serialize();
    emit paperprintChanged();
}


bool Parameters::getMailActive() const
{
    return m_mailActive;
}

void Parameters::setMailActive(bool mailActive)
{
    m_mailActive = mailActive;
    Serialize();
    emit mailChange();
}

QString Parameters::getMailFrom() const
{
    return m_mailFrom;
}

void Parameters::setMailFrom(const QString &mailFrom)
{
    m_mailFrom = mailFrom;
    Serialize();
    emit mailChange();
}

QString Parameters::getMailCc() const
{
    return m_mailCc;
}

void Parameters::setMailCc(const QString &mailCc)
{
    m_mailCc = mailCc;
    Serialize();
    emit mailChange();
}

QString Parameters::getMailBcc() const
{
    return m_mailBcc;
}

void Parameters::setMailBcc(const QString &mailBcc)
{
    m_mailBcc = mailBcc;
    Serialize();
    emit mailChange();
}

QString Parameters::getMailSmtp() const
{
    return m_mailSmtp;
}

void Parameters::setMailSmtp(const QString &mailSmtp)
{
    m_mailSmtp = mailSmtp;
    Serialize();
    emit mailChange();
}

QString Parameters::getMailPort() const
{
    return m_mailPort;
}

void Parameters::setMailPort(const QString &mailPort)
{
    m_mailPort = mailPort;
    Serialize();
    emit mailChange();
}

QString Parameters::getMailUsername() const
{
    return m_mailUsername;
}

void Parameters::setMailUsername(const QString &mailUsername)
{
    m_mailUsername = mailUsername;
    Serialize();
    emit mailChange();
}

QString Parameters::getMailPassword() const
{
    return m_mailPassword;
}

void Parameters::setMailPassword(const QString &mailPassword)
{
    m_mailPassword = mailPassword;
    Serialize();
    emit mailChange();
}

QString Parameters::getMailSubject() const
{
    return m_mailSubject;
}

void Parameters::setMailSubject(const QString &mailSubject)
{
    m_mailSubject = mailSubject;
    Serialize();
    emit mailChange();
}

void Parameters::setMailContent(const QString &mailContent)
{
    m_mailContent = mailContent;
    Serialize();
    emit mailChange();
}

QString Parameters::getMailContent() const
{
    return m_mailContent;
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
