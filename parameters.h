#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <QObject>
#include <QList>
#include <QVariant>

#include "photo.h"
#include "photogallery.h"
#include "videoitem.h"
#include "photogallery.h"
#include "common.h"
#include "parameters.h"
#include "photoqueuemanager.h"

class PhotoGallery;
class VideoItem;
class PhotoQueueManager;
class Parameters : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<QObject*> templates READ getTemplates WRITE setTemplates NOTIFY templatesChanged)
    Q_PROPERTY(QList<QObject*> activesTemplates READ getActivesTemplates WRITE setActivesTemplates NOTIFY activeTemplatesChanged)
    Q_PROPERTY(QList<QObject*> effects READ getEffects WRITE setEffects NOTIFY effectsChanged)
    Q_PROPERTY(QList<QObject*> activeEffects READ getActivesEffects WRITE setActivesEffects NOTIFY effectsSelectedChanged)
    Q_PROPERTY(QString  effectDefault READ getEffectDefault WRITE setEffectDefault NOTIFY effectDefaultChanged)
    Q_PROPERTY(PhotoGallery* photoGallery READ getPhotogallery WRITE setPhotogallery NOTIFY photoGalleryChanged)
    Q_PROPERTY(QUrl applicationDirPath READ getApplicationDirPath WRITE setApplicationDirPath NOTIFY applicationDirPathChanged)
    Q_PROPERTY(int nbprint READ getNbprint WRITE setNbprint NOTIFY nbPrintChanged)
    Q_PROPERTY(int nbfreephotos READ getNbfreephotos WRITE setNbfreephotos NOTIFY nbfreephotoChanged)
    Q_PROPERTY(float pricephoto READ getPricephoto WRITE setPricephoto NOTIFY pricephotoChanged)
    Q_PROPERTY(bool flipcamera READ getFlipcamera WRITE setFlipcamera NOTIFY flipcameraChanged)
    Q_PROPERTY(bool flipresult READ getFlipresult WRITE setFlipresult NOTIFY flipresultChanged)
    Q_PROPERTY(QString backgroundImage READ getBackgroundImage WRITE setBackgroundImage NOTIFY backgroundImageChange)
    Q_PROPERTY(int cameraHeight READ getCameraHight WRITE setCameraHight NOTIFY cameraHeightChange)
    Q_PROPERTY(int cameraWidth READ getCameraWidth WRITE setCameraWidth NOTIFY cameraWidthChange)
    Q_PROPERTY(bool blockPrint READ getBlockPrint WRITE setBlockPrint NOTIFY blockPrintChanged)
    Q_PROPERTY(int blockPrintNb READ getBlockPrintNb WRITE setBlockPrintNb NOTIFY blockPrintNbChanged)
    Q_PROPERTY(int paperprint READ paperprint WRITE setPaperprint NOTIFY paperprintChanged)
    Q_PROPERTY(bool autoPrint READ getAutoPrint WRITE setAutoPrint NOTIFY autoPrintChanged)
    Q_PROPERTY(int autoPrintDelay READ getAutoPrintDelay WRITE setAutoPrintDelay NOTIFY autoPrintDelayChanged)
    Q_PROPERTY(int showPhotoDelay READ getShowPhotoDelay WRITE setShowPhotoDelay NOTIFY showPhotoDelayChanged)
    Q_PROPERTY(bool mailActive READ getMailActive WRITE setMailActive NOTIFY mailChange)
    Q_PROPERTY(QString mailFrom READ getMailFrom WRITE setMailFrom NOTIFY mailChange)
    Q_PROPERTY(QString mailCc READ getMailCc WRITE setMailCc NOTIFY mailChange)
    Q_PROPERTY(QString mailBcc READ getMailBcc WRITE setMailBcc NOTIFY mailChange)
    Q_PROPERTY(QString mailSmtp READ getMailSmtp WRITE setMailSmtp NOTIFY mailChange)
    Q_PROPERTY(QString mailPort READ getMailPort WRITE setMailPort NOTIFY mailChange)
    Q_PROPERTY(QString mailUsername READ getMailUsername WRITE setMailUsername NOTIFY mailChange)
    Q_PROPERTY(QString mailPassword READ getMailPassword WRITE setMailPassword NOTIFY mailChange)
    Q_PROPERTY(QString mailSubject READ getMailSubject WRITE setMailSubject NOTIFY mailChange)
    Q_PROPERTY(QString mailContent READ getMailContent WRITE setMailContent NOTIFY mailChange)
    Q_PROPERTY(VideoItem* waitVideo READ waitVideo WRITE setWaitVideo NOTIFY parameterVideoChanged)
    Q_PROPERTY(VideoItem* startGlobalPhotoProcessVideo READ startGlobalPhotoProcessVideo WRITE setStartGlobalPhotoProcessVideo NOTIFY parameterVideoChanged)
    Q_PROPERTY(VideoItem* endGlobalPhotoProcessVideo READ endGlobalPhotoProcessVideo WRITE setEndGlobalPhotoProcessVideo NOTIFY parameterVideoChanged)
    Q_PROPERTY(VideoItem* startPhotoProcessVideo READ startPhotoProcessVideo WRITE setStartPhotoProcessVideo NOTIFY parameterVideoChanged)
    Q_PROPERTY(bool twitterTwitterOnly READ getTwitterTwitterOnly WRITE setTwitterTwitterOnly NOTIFY twitterTwitterOnlyChanged)
    Q_PROPERTY(bool twitterListenTwitter READ getTwitterListenTwitter WRITE setTwitterListenTwitter NOTIFY twitterListenTwitterChanged)
    Q_PROPERTY(QString twitterKey READ getTwitterKey WRITE setTwitterKey NOTIFY twitterKeyChanged)
    Q_PROPERTY(QString twitterSecret READ getTwitterSecret WRITE setTwitterSecret NOTIFY twitterSecretChanged)
    Q_PROPERTY(QString twitterAccount READ getTwitterAccount WRITE setTwitterAccount NOTIFY twitterAccountChanged)
    Q_PROPERTY(QString twitterTag READ getTwitterTag WRITE setTwitterTag NOTIFY twitterTagChanged)
    Q_PROPERTY(Template * twitterDefaultTemplate READ getTwitterDefaultTemplate WRITE setTwitterDefaultTemplate NOTIFY twitterDefaultTemplateChanged)
    Q_PROPERTY(QString twitterLastRetrieved READ getTwitterLastRetrieved WRITE setTwitterLastRetrieved NOTIFY twitterLastRetrievedChanged)
    Q_PROPERTY(QString twitterMessage READ getTwitterMessage WRITE setTwitterMessage NOTIFY twitterMessageChanged)
    Q_PROPERTY(QString twitterMessageColor READ getTwitterMessageColor WRITE setTwitterMessageColor NOTIFY twitterMessageColorChanged)
    Q_PROPERTY(PhotoQueueManager * photoQueueManager READ getPhotoQueueManager WRITE setPhotoQueueManager NOTIFY photoQueueManagerChanged)
public:
    Parameters(QUrl appDirPath);
    ~Parameters();

    Q_INVOKABLE void    Serialize();
    //Q_INVOKABLE Photo*  addPhotoToGallerie(QString name, QObject *temp);
    Q_INVOKABLE void    printPhoto(QUrl url, bool doubleprint, bool cutprint, bool landscape);
    Q_INVOKABLE void    clearGallery();
    Q_INVOKABLE void    clearGalleryDeleteImages();
    Q_INVOKABLE void    addTemplateFromUrl(QUrl url);
    Q_INVOKABLE void    deleteTemplateFromName(QString name);
    Q_INVOKABLE void    changeBackground(QUrl url);
    Q_INVOKABLE void    updatePaperPrint();
    Q_INVOKABLE void    haltSystem();
    Q_INVOKABLE void    updateEffect(QString name, bool active, bool twitterDefault);

    QList<QObject*> getTemplates();
    void setTemplates(QList<QObject*> templates);
    QList<QObject *> getActivesTemplates() const;
    void setActivesTemplates(const QList<QObject *> &activesTemplates);
    void rebuildActivesTemplates();

    //Accessprs
    PhotoGallery *getPhotogallery() const;
    void setPhotogallery(PhotoGallery *photogallery);

    QUrl getApplicationDirPath() const;
    void setApplicationDirPath(const QUrl &applicationDirPath);

    int getNbprint() const;
    void setNbprint(int nbprint);

    int getNbfreephotos() const;
    void setNbfreephotos(int nbfreephotos);

    float getPricephoto() const;
    void setPricephoto(float pricephoto);

    bool getFlipcamera() const;
    void setFlipcamera(bool flipcamera);

    bool getFlipresult() const;
    void setFlipresult(bool flipresult);

    QString getBackgroundImage() const;
    void setBackgroundImage(const QString &backgroundImage);

    int getCameraHight() const;
    void setCameraHight(int cameraHight);

    int getCameraWidth() const;
    void setCameraWidth(int cameraWidth);

    bool getBlockPrint() const;
    void setBlockPrint(bool blockPrint);

    int getBlockPrintNb() const;
    void setBlockPrintNb(int blockPrintNb);

    int paperprint() const;
    void setPaperprint(int paperprint);

    bool getMailActive() const;
    void setMailActive(bool mailActive);

    QString getMailFrom() const;
    void setMailFrom(const QString &mailFrom);

    QString getMailCc() const;
    void setMailCc(const QString &mailCc);

    QString getMailBcc() const;
    void setMailBcc(const QString &mailBcc);

    QString getMailSmtp() const;
    void setMailSmtp(const QString &mailSmtp);

    QString getMailPort() const;
    void setMailPort(const QString &mailPort);

    QString getMailUsername() const;
    void setMailUsername(const QString &mailUsername);

    QString getMailPassword() const;
    void setMailPassword(const QString &mailPassword);

    QString getMailSubject() const;
    void setMailSubject(const QString &mailSubject);

    void setMailContent(const QString &mailContent);
    QString getMailContent() const;

    VideoItem *waitVideo() const;
    void setWaitVideo(VideoItem *waitVideo);

    VideoItem *startPhotoProcessVideo() const;
    void setStartPhotoProcessVideo(VideoItem *startPhotoProcessVideo);

    VideoItem *startGlobalPhotoProcessVideo() const;
    void setStartGlobalPhotoProcessVideo(VideoItem *startGlobalPhotoProcessVideo);

    VideoItem *endGlobalPhotoProcessVideo() const;
    void setEndGlobalPhotoProcessVideo(VideoItem *endGlobalPhotoProcessVideo);

    bool getTwitterTwitterOnly() const;
    void setTwitterTwitterOnly(bool twitterTwitterOnly);

    bool getTwitterListenTwitter() const;
    void setTwitterListenTwitter(bool twitterListenTwitter);

    QString getTwitterKey() const;
    void setTwitterKey(const QString &twitterKey);

    QString getTwitterSecret() const;
    void setTwitterSecret(const QString &twitterSecret);

    QString getTwitterAccount() const;
    void setTwitterAccount(const QString &twitterAccount);

    QString getTwitterTag() const;
    void setTwitterTag(const QString &twitterTag);

    QList<QObject *> getEffects() const;
    void setEffects(const QList<QObject *> &effects);

    QList<QObject *> getActivesEffects() const;
    void setActivesEffects(const QList<QObject *> &activesEffects);

    Template *getTwitterDefaultTemplate() const;
    void setTwitterDefaultTemplate(Template *twitterDefaultTemplate);

    QString getEffectDefault() const;
    void setEffectDefault(const QString &effectDefault);

    bool getAutoPrint() const;
    void setAutoPrint(bool autoPrint);

    QString getTwitterLastRetrieved() const;
    void setTwitterLastRetrieved(const QString &twitterLastRetrieved);

    int getAutoPrintDelay() const;
    void setAutoPrintDelay(int autoPrintDelay);

    int getShowPhotoDelay() const;
    void setShowPhotoDelay(int showPhotoDelay);

    QString getTwitterMessage() const;
    void setTwitterMessage(const QString &twitterMessage);

    QString getTwitterMessageColor() const;
    void setTwitterMessageColor(const QString &twitterMessageColor);

    PhotoQueueManager *getPhotoQueueManager() const;
    void setPhotoQueueManager(PhotoQueueManager *photoQueueManager);

private:
    void addTemplate(QString name);
    void addTemplate(Value const &value);
    void init();
    void initEffects();
    void readTemplateDir();
    void Unserialize();
    void createFolders();
    void clearGallery(bool del);
    void printThread(QUrl m_applicationDirPath, QUrl url, bool doubleprint, bool cutprint, bool landscape);


    std::vector<QString> m_effectNameList = {   "Couleur",
                                                    "Sepia",
                                                    "Edge",
                                                    "Inkwell",
                                                    "1977",
                                                    "Amaro",
                                                    "Branna",
                                                    "Early Bird",
                                                    "Hefe",
                                                    "Hudson",
                                                    "Lomo",
                                                    "Lord Kelvin",
                                                    "Nashville",
                                                    "Pixel",
                                                    "Rise",
                                                    "Sierra",
                                                    "Sutro",
                                                    "Toaster",
                                                    "Valancia",
                                                    "Walden",
                                                    "XPro"};

    QUrl                 m_applicationDirPath;
    QList<QObject*>      m_templates;
    QList<QObject*>      m_activesTemplates;
    QList<QObject*>      m_effects;
    QList<QObject*>      m_activesEffects;
    PhotoGallery*        m_photogallery;
    int                  m_nbprint;
    int                  m_nbfreephotos;
    bool                 m_blockPrint;
    int                  m_blockPrintNb;
    float                m_pricephoto;
    bool                 m_autoPrint;
    int                  m_autoPrintDelay;
    int                  m_showPhotoDelay;
    bool                 m_flipcamera;
    bool                 m_flipresult;
    int                  m_cameraHight;
    int                  m_cameraWidth;
    int                  m_paperprint;
    QString              m_backgroundImage;
    bool                 m_mailActive;
    QString              m_mailFrom;
    QString              m_mailCc;
    QString              m_mailBcc;
    QString              m_mailSmtp;
    QString              m_mailPort;
    QString              m_mailUsername;
    QString              m_mailPassword;
    QString              m_mailSubject;
    QString              m_mailContent;
    VideoItem *          m_waitVideo;
    VideoItem *          m_startGlobalPhotoProcessVideo;
    VideoItem *          m_startPhotoProcessVideo;
    VideoItem *          m_endGlobalPhotoProcessVideo;
    bool                 m_twitterTwitterOnly;
    bool                 m_twitterListenTwitter;
    QString              m_twitterKey;
    QString              m_twitterSecret;
    QString              m_twitterAccount;
    QString              m_twitterTag;
    Template *           m_twitterDefaultTemplate;
    QString              m_twitterLastRetrieved;
    QString              m_twitterMessage;
    QString              m_twitterMessageColor;
    QString              m_effectDefault;

    PhotoQueueManager *  m_photoQueueManager;


signals:
    void templatesChanged();
    void activeTemplatesChanged();
    void photoGalleryChanged();
    void photoGalleryListChanged();
    void applicationDirPathChanged();
    void nbPrintChanged();
    void nbfreephotoChanged();
    void pricephotoChanged();
    void flipcameraChanged();
    void flipresultChanged();
    void backgroundImageChange();
    void cameraHeightChange();
    void cameraWidthChange();
    void blockPrintChanged();
    void blockPrintNbChanged();
    void paperprintChanged();
    void mailChange();
    void countdownDelayChange();
    void parameterVideoChanged();
    void twitterTwitterOnlyChanged();
    void twitterListenTwitterChanged();
    void twitterKeyChanged();
    void twitterSecretChanged();
    void twitterAccountChanged();
    void twitterTagChanged();
    void effectsChanged();
    void effectsSelectedChanged();
    void twitterDefaultTemplateChanged();
    void effectDefaultChanged();
    void twitterLastRetrievedChanged();
    void autoPrintChanged();
    void autoPrintDelayChanged();
    void showPhotoDelayChanged();
    void twitterMessageColorChanged();
    void twitterMessageChanged();
    void photoQueueManagerChanged();
};

#endif // PARAMETERS_H
