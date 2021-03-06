#include <QApplication>
#include <QCursor>
#include <QQmlEngine>
#include <QDir>
#include <QtConcurrent>
#include <QFile>
#include <QProcess>
#include <iostream>
#include <fstream>
#include "parameters.h"
#include "effect.h"

#include "rapidjson/document.h"
#include "clog.h"
#include "cameraworker.h"

using namespace std;
using namespace rapidjson;

/**
 * Constructor / Destructor / init
 */

Parameters::Parameters(QUrl appDirPath)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    m_applicationDirPath = appDirPath;
    init();
}

Parameters::~Parameters()
{
    CLog::Write(CLog::Debug, "Delete parameters");
}

void Parameters::init() {
    m_nbprint = 0;
    m_nbfreephotos = 250;
    m_pricephoto = 0.4;
    m_flipcamera = false;
    m_flipresult = false;
    m_backgroundImage = "";
    m_cameraHight = CAMERA_HEIGHT;
    m_cameraWidth = CAMERA_WIDTH;
    m_blockPrint = false;
    m_blockPrintNb = 700;
    createFolders();
    m_photogallery = new PhotoGallery(this);
    QQmlEngine::setObjectOwnership(m_photogallery, QQmlEngine::CppOwnership);
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
    m_twitterListenTwitter = true;
    m_twitterTwitterOnly = false;
    m_twitterAccount = "";
    m_twitterKey = "";
    m_twitterSecret = "";
    m_twitterTag = "";
    m_twitterLastRetrieved = "";
    m_twitterMessage = "";
    m_twitterMessageColor = "#00ACED";
    m_autoPrintDelay = 5;
    m_autoPrint = false;
    m_showPhotoDelay = 15;
    m_mirrorConnected = false;
    m_countdown = 5;
    m_viewPhotoTime = 5;
    m_viewAllPhotoTime = 5;
    m_isMirror = false;
    m_commandIP = "192.168.30.1";
    m_mirrorIP = "192.168.30.2";
    m_waitVideoRepeatBeforeTwitter = 2;


    m_waitVideo = new VideoItem(this, VIDEO_WAIT);
    QQmlEngine::setObjectOwnership(m_waitVideo, QQmlEngine::CppOwnership);
    m_twitterVideo = new VideoItem(this, VIDEO_TWITTER);
    QQmlEngine::setObjectOwnership(m_twitterVideo, QQmlEngine::CppOwnership);
    m_startGlobalPhotoProcessVideo = new VideoItem(this, VIDEO_STARTGLOBALPHOTOPROCESS);
    QQmlEngine::setObjectOwnership(m_startGlobalPhotoProcessVideo, QQmlEngine::CppOwnership);
    m_startPhotoProcessVideo = new VideoItem(this, VIDEO_STARTPHOTOPROCESS);
    QQmlEngine::setObjectOwnership(m_startPhotoProcessVideo, QQmlEngine::CppOwnership);
    m_endGlobalPhotoProcessVideo = new VideoItem(this, VIDEO_ENDGLOBALPHOTOPROCESS);
    QQmlEngine::setObjectOwnership(m_endGlobalPhotoProcessVideo, QQmlEngine::CppOwnership);

    m_photoQueueManager = new PhotoQueueManager(this);
    QQmlEngine::setObjectOwnership(m_photoQueueManager, QQmlEngine::CppOwnership);
    m_wifiManager = new WifiManager(this);
    QQmlEngine::setObjectOwnership(m_wifiManager, QQmlEngine::CppOwnership);
    m_raspiGPIO = new RaspiGPIO();

    m_printerManager = new PrinterManager(this);
    QQmlEngine::setObjectOwnership(m_printerManager, QQmlEngine::CppOwnership);

    initEffects();

    Unserialize(); //Unserialize parameters
    m_photogallery->Unserialize(m_templates); //unserialize current gallery

    //Read all .png and .jpg files in tempalte directory
    readTemplateDir();
    m_printerManager->updatePaperPrint();
    Serialize();
    rebuildActivesTemplates();
}

void Parameters::initEffects()
{
    for (unsigned int i = 0; i < m_effectNameList.size(); i++) {
        QString effectName = m_effectNameList[i];
        Effect *effect = new Effect(effectName);
        QQmlEngine::setObjectOwnership(effect, QQmlEngine::CppOwnership);
        m_effects.append(effect);
    }
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

        writer.Key("autoPrint");
        writer.Bool(m_autoPrint);

        writer.Key("autoPrintDelay");
        writer.Int(m_autoPrintDelay);

        writer.Key("showPhotoDelay");
        writer.Int(m_showPhotoDelay);

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

        writer.Key("twitterTwitterOnly");
        writer.Bool(m_twitterTwitterOnly);

        writer.Key("twitterListenTwitter");
        writer.Bool(m_twitterListenTwitter);

        writer.Key("twitterKey");
        writer.String(m_twitterKey.toStdString().c_str());

        writer.Key("twitterSecret");
        writer.String(m_twitterSecret.toStdString().c_str());

        writer.Key("twitterAccount");
        writer.String(m_twitterAccount.toStdString().c_str());

        writer.Key("twitterTag");
        writer.String(m_twitterTag.toStdString().c_str());

        writer.Key("twitterLastRetrieved");
        writer.String(m_twitterLastRetrieved.toStdString().c_str());

        writer.Key("twitterMessage");
        writer.String(m_twitterMessage.toStdString().c_str());

        writer.Key("twitterMessageColor");
        writer.String(m_twitterMessageColor.toStdString().c_str());

        writer.Key("countdown");
        writer.Int(m_countdown);

        writer.Key("viewPhotoTime");
        writer.Int(m_viewPhotoTime);

        writer.Key("viewAllPhotoTime");
        writer.Int(m_viewAllPhotoTime);

        writer.Key("isMirror");
        writer.Bool(m_isMirror);

        writer.Key("commandIP");
        writer.String(m_commandIP.toStdString().c_str());

        writer.Key("mirrorIP");
        writer.String(m_mirrorIP.toStdString().c_str());

        writer.Key("waitVideoRepeatBeforeTwitter");
        writer.Int(m_waitVideoRepeatBeforeTwitter);

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

        writer.Key("effects");
        writer.StartArray();
        for (QList<QObject*>::const_iterator it = m_effects.begin(); it != m_effects.end(); it++) {
            if (Effect *e = dynamic_cast<Effect*>(*it)) {
                e->Serialize(writer, this);
            } else {
                CLog::Write(CLog::Fatal, "Bad type QObject -> Effect");
            }
        }
        writer.EndArray();

        writer.Key("videos");
        writer.StartArray();
        m_waitVideo->serialize(writer);
        m_twitterVideo->serialize(writer);
        m_startGlobalPhotoProcessVideo->serialize(writer);
        m_startPhotoProcessVideo->serialize(writer);
        m_endGlobalPhotoProcessVideo->serialize(writer);
        writer.EndArray();

        writer.Key("wifis");
        writer.StartArray();
        m_wifiManager->serialize(writer);
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

    if (document.HasMember("autoPrint")) {
       m_autoPrint = document["autoPrint"].GetBool();
    }

    if (document.HasMember("autoPrintDelay")) {
        m_autoPrintDelay = document["autoPrintDelay"].GetInt();
    }

    if (document.HasMember("showPhotoDelay")) {
        m_showPhotoDelay = document["showPhotoDelay"].GetInt();
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

    if (document.HasMember("twitterTwitterOnly")) {
        m_twitterTwitterOnly = document["twitterTwitterOnly"].GetBool();
    }

    if (document.HasMember("twitterListenTwitter")) {
        m_twitterListenTwitter = document["twitterListenTwitter"].GetBool();
    }

    if (document.HasMember("twitterKey")) {
        m_twitterKey = QString(document["twitterKey"].GetString());
    }

    if (document.HasMember("twitterSecret")) {
        m_twitterSecret = QString(document["twitterSecret"].GetString());
    }

    if (document.HasMember("twitterAccount")) {
        m_twitterAccount = QString(document["twitterAccount"].GetString());
    }

    if (document.HasMember("twitterTag")) {
        m_twitterTag = QString(document["twitterTag"].GetString());
    }

    if (document.HasMember("twitterLastRetrieved")) {
        m_twitterLastRetrieved = QString(document["twitterLastRetrieved"].GetString());
    }

    if (document.HasMember("twitterMessage")) {
        m_twitterMessage = QString(document["twitterMessage"].GetString());
    }

    if (document.HasMember("twitterMessageColor")) {
        m_twitterMessageColor = QString(document["twitterMessageColor"].GetString());
    }

    if (document.HasMember("countdown")) {
        m_countdown = document["countdown"].GetInt();
    }

    if (document.HasMember("viewPhotoTime")) {
        m_viewPhotoTime = document["viewPhotoTime"].GetInt();
    }

    if (document.HasMember("viewAllPhotoTime")) {
        m_viewAllPhotoTime = document["viewAllPhotoTime"].GetInt();
    }

    if (document.HasMember("isMirror")) {
        m_isMirror = document["isMirror"].GetBool();
    }

    if (document.HasMember("commandIP")) {
        m_commandIP = document["commandIP"].GetString();
    }

    if (document.HasMember("mirrorIP")) {
        m_mirrorIP = document["mirrorIP"].GetString();
    }

    if (document.HasMember("waitVideoRepeatBeforeTwitter")) {
        m_waitVideoRepeatBeforeTwitter = document["waitVideoRepeatBeforeTwitter"].GetInt();
    }

    if (document.HasMember("templates")) {
        const Value& templates = document["templates"];
        if (templates.IsArray()) {
            for (SizeType i = 0; i < templates.Size(); i++) {
                addTemplate(templates[i]);
            }
        }
    }

    if (document.HasMember("effects")) {
        const Value& effects = document["effects"];
        if (effects.IsArray()) {
            for (SizeType i = 0; i < effects.Size(); i++) {
                Value const &value = effects[i];
                QString effectName = "";
                bool effectEnable = true;
                bool effectTwitterDefault = false;

                if (value.HasMember("effect_name")) {
                    effectName = QString(value["effect_name"].GetString());
                }

                if (value.HasMember("effect_enable")) {
                    effectEnable = value["effect_enable"].GetBool();
                }

                if (value.HasMember("effect_twitterDefault")) {
                    effectTwitterDefault = value["effect_twitterDefault"].GetBool();
                }
                updateEffect(effectName, effectEnable, effectTwitterDefault);
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
                        case VIDEO_TWITTER:
                            m_twitterVideo->unserialize(videos[i]);
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

    //Restore known wifi password with at least one connection
    if (document.HasMember("wifis")) {
        const Value& wifis = document["wifis"];
        if (wifis.IsArray()) {
            m_wifiManager->unserialize(wifis);
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

void Parameters::haltSystem()
{
    QProcess haltProcess;

    QString program = "sudo";
    QStringList arguments;
    arguments << "halt";
    haltProcess.start(program, arguments);
    haltProcess.waitForFinished();
}

PrinterManager *Parameters::getPrinterManager() const
{
    return m_printerManager;
}

void Parameters::setPrinterManager(PrinterManager *printerManager)
{
    m_printerManager = printerManager;
}

RaspiGPIO *Parameters::getRaspiGPIO() const
{
    return m_raspiGPIO;
}

void Parameters::setRaspiGPIO(RaspiGPIO *raspiGPIO)
{
    m_raspiGPIO = raspiGPIO;
}

WifiManager *Parameters::getWifiManager() const
{
    return m_wifiManager;
}

void Parameters::setWifiManager(WifiManager *wifiManager)
{
    m_wifiManager = wifiManager;
}

PhotoQueueManager *Parameters::getPhotoQueueManager() const
{
    return m_photoQueueManager;
}

void Parameters::setPhotoQueueManager(PhotoQueueManager *photoQueueManager)
{
    m_photoQueueManager = photoQueueManager;
}

QString Parameters::getTwitterMessageColor() const
{
    return m_twitterMessageColor;
}

void Parameters::setTwitterMessageColor(const QString &twitterMessageColor)
{
    m_twitterMessageColor = twitterMessageColor;
    emit twitterMessageColorChanged();
}

QString Parameters::getTwitterMessage() const
{
    QString tmp(m_twitterMessage);
    return tmp.replace("\\n", "\n");
}

void Parameters::setTwitterMessage(const QString &twitterMessage)
{
    m_twitterMessage = twitterMessage;
    emit twitterMessageChanged();
}

int Parameters::getShowPhotoDelay() const
{
    return m_showPhotoDelay;
}

void Parameters::setShowPhotoDelay(int showPhotoDelay)
{
    m_showPhotoDelay = showPhotoDelay;
    emit showPhotoDelayChanged();
    Serialize();
}

int Parameters::getAutoPrintDelay() const
{
    return m_autoPrintDelay;
}

void Parameters::setAutoPrintDelay(int autoPrintDelay)
{
    m_autoPrintDelay = autoPrintDelay;
    emit autoPrintDelayChanged();
    Serialize();
}

QString Parameters::getTwitterLastRetrieved() const
{
    return m_twitterLastRetrieved;
}

void Parameters::setTwitterLastRetrieved(const QString &twitterLastRetrieved)
{
    m_twitterLastRetrieved = twitterLastRetrieved;
    emit twitterLastRetrievedChanged();
    Serialize();
}

bool Parameters::getAutoPrint() const
{
    return m_autoPrint;
}

void Parameters::setAutoPrint(bool autoPrint)
{
    m_autoPrint = autoPrint;
    emit autoPrintChanged();
    Serialize();
}

QString Parameters::getEffectDefault() const
{
    for (QList<QObject *>::const_iterator it = m_effects.begin(); it != m_effects.end(); it++) {
        if (Effect *t = dynamic_cast<Effect*>(*it)) {
            if (t->getEffectTwitterDefault()) {
                return t->getEffectName();
            }
        }
    }

    return "Couleur";
}

void Parameters::setEffectDefault(const QString &effectDefault)
{
    m_effectDefault = effectDefault;
}

Template *Parameters::getTwitterDefaultTemplate() const
{
    for (QList<QObject *>::const_iterator it = m_templates.begin(); it != m_templates.end(); it++) {
        if (Template *t = dynamic_cast<Template*>(*it)) {
            if (t->getTwitterDefault()) {
                return t;
            }
        }
    }

    return NULL;
}

void Parameters::setTwitterDefaultTemplate(Template *twitterDefaultTemplate)
{
    CLog::Write(CLog::Debug, "Set default template:" + twitterDefaultTemplate->getName());
    m_twitterDefaultTemplate = twitterDefaultTemplate;
    emit twitterDefaultTemplateChanged();
}

void Parameters::updateEffect(QString name, bool active, bool twitterDefault)
{
    //Parcour de tout les effects jusqu'a trouver celui avec le même nom
    for (QList<QObject*>::iterator it = m_effects.begin(); it != m_effects.end(); it++) {
        if (Effect *t = dynamic_cast<Effect*>(*it)) {
            if (t->getEffectName() == name) {
                t->setEffectEnable(active);
                t->setEffectTwitterDefault(twitterDefault);

                if (active) {
                    bool find = false;
                    //On regarde si il est deja present
                    for (QList<QObject*>::iterator it = m_activesEffects.begin(); it != m_activesEffects.end(); it++) {
                        if (Effect *at = dynamic_cast<Effect*>(*it)) {
                            if (at->getEffectName() == name) {
                                //On l'a trouvé, on le met a jour
                                find = true;
                                at->setEffectEnable(active);
                                at->setEffectTwitterDefault(twitterDefault);
                                emit effectsSelectedChanged();
                            }
                        }
                    }
                    //Sinon on l'ajoute
                    if (!find) {
                        m_activesEffects.append(t);
                        emit effectsSelectedChanged();
                    }
                } else {
                    //Si il est présent on le supprime
                    m_activesEffects.removeAll(t);
                    emit effectsSelectedChanged();
                }

                break;
            }
        }
    }
}

void Parameters::showCursor()
{
    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
}

void Parameters::hideCursor()
{
    QApplication::setOverrideCursor(QCursor(Qt::BlankCursor));
}

QList<QObject *> Parameters::getActivesEffects() const
{
    return m_activesEffects;
}

void Parameters::setActivesEffects(const QList<QObject *> &activesEffects)
{
    m_activesEffects = activesEffects;
}

QList<QObject *> Parameters::getEffects() const
{
    return m_effects;
}

void Parameters::setEffects(const QList<QObject *> &effects)
{
    m_effects = effects;
}

QString Parameters::getTwitterTag() const
{
    return m_twitterTag;
}

void Parameters::setTwitterTag(const QString &twitterTag)
{
    m_twitterTag = twitterTag;
}

QString Parameters::getTwitterAccount() const
{
    return m_twitterAccount;
}

void Parameters::setTwitterAccount(const QString &twitterAccount)
{
    m_twitterAccount = twitterAccount;
}

QString Parameters::getTwitterSecret() const
{
    return m_twitterSecret;
}

void Parameters::setTwitterSecret(const QString &twitterSecret)
{
    m_twitterSecret = twitterSecret;
}

QString Parameters::getTwitterKey() const
{
    return m_twitterKey;
}

void Parameters::setTwitterKey(const QString &twitterKey)
{
    m_twitterKey = twitterKey;
}

bool Parameters::getTwitterListenTwitter() const
{
    return m_twitterListenTwitter;
}

void Parameters::setTwitterListenTwitter(bool twitterListenTwitter)
{
    m_twitterListenTwitter = twitterListenTwitter;
    emit twitterListenTwitterChanged();
}

bool Parameters::getTwitterTwitterOnly() const
{
    return m_twitterTwitterOnly;
}

void Parameters::setTwitterTwitterOnly(bool twitterTwitterOnly)
{
    m_twitterTwitterOnly = twitterTwitterOnly;
    emit twitterTwitterOnlyChanged();
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
                    furl = t->getUrl().toLocalFile();
                }
                if (!QFile::remove(furl)) {
                    CLog::Write(CLog::Error, "Unable to delete file : " + t->getUrl().toString().toStdString());
                }
                //Delete template from library
                //m_templates.removeOne(*it);
                m_templates.removeAt(it - m_templates.begin());

                //Serialize
                Serialize();

                //Update interface
                emit templatesChanged();
                break ;
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
    /*
    delete m_photogallery;
    m_photogallery = new PhotoGallery(this);
    QQmlEngine::setObjectOwnership(m_photogallery, QQmlEngine::CppOwnership);
    m_photogallery->setApplicationDirPath(m_applicationDirPath);
    */
    m_photogallery->clearGallery();


    if (del) {
        delAllFileInDirectory(PHOTOS_PATH);
        delAllFileInDirectory(PHOTOSSD_PATH);
        delAllFileInDirectory(PHOTOSS_PATH);
        delAllFileInDirectory(PHOTOSD_PATH);
        delAllFileInDirectory(PHOTOSDS_PATH);
    }

    m_twitterLastRetrieved = "";
    Serialize();
    m_photogallery->Serialize();
    emit photoGalleryChanged();
}

int Parameters::getWaitVideoRepeatBeforeTwitter() const
{
    return m_waitVideoRepeatBeforeTwitter;
}

void Parameters::setWaitVideoRepeatBeforeTwitter(int waitVideoRepeatBeforeTwitter)
{
    m_waitVideoRepeatBeforeTwitter = waitVideoRepeatBeforeTwitter;
    emit waitVideoRepeatBeforeTwitterChanged();
    Serialize();
}

QString Parameters::getMirrorIP() const
{
    return m_mirrorIP;
}

void Parameters::setMirrorIP(const QString &mirrorIP)
{
    m_mirrorIP = mirrorIP;
    emit mirrorIPChanged();
    Serialize();
}

VideoItem *Parameters::getTwitterVideo() const
{
    return m_twitterVideo;
}

void Parameters::setTwitterVideo(VideoItem *twitterVideo)
{
    m_twitterVideo = twitterVideo;
}

QString Parameters::getCommandIP() const
{
    return m_commandIP;
}

void Parameters::setCommandIP(const QString &commandIP)
{
    m_commandIP = commandIP;
    emit commandIPChanged();
    Serialize();
}

bool Parameters::getIsMirror() const
{
    return m_isMirror;
}

void Parameters::setIsMirror(bool isMirror)
{
    m_isMirror = isMirror;
    emit isMirrorChanged();
    Serialize();
}

int Parameters::getViewAllPhotoTime() const
{
    return m_viewAllPhotoTime;
}

void Parameters::setViewAllPhotoTime(int viewAllPhotoTime)
{
    m_viewAllPhotoTime = viewAllPhotoTime;
    emit viewAllPhotoTimeChanged();
    Serialize();
}

int Parameters::getViewPhotoTime() const
{
    return m_viewPhotoTime;
}

void Parameters::setViewPhotoTime(int viewPhotoTime)
{
    m_viewPhotoTime = viewPhotoTime;
    emit viewPhotoTimeChanged();
    Serialize();
}

int Parameters::getCountdown() const
{
    return m_countdown;
}

void Parameters::setCountdown(int countdown)
{
    m_countdown = countdown;
    emit countdownChanged();
    Serialize();
}

bool Parameters::getMirrorConnected() const
{
    return m_mirrorConnected;
}

void Parameters::setMirrorConnected(bool mirrorConnected)
{
    m_mirrorConnected = mirrorConnected;
    emit mirrorConnectedChanged();
}

/*
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
*/
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
