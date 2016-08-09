#ifndef PHOTO_H
#define PHOTO_H

#include <QObject>

#include "templatephotoposition.h"
#include "photopart.h"
#include "template.h"

class Template;
class PhotoPart;
class TemplatePhotoPosition;
class Photo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl finalResult READ finalResult WRITE setFinalResult NOTIFY finalResultChanged)
    Q_PROPERTY(QUrl finalResultSD READ finalResultSD WRITE setFinalResultSD NOTIFY finalResultSDChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QList<QObject*> photoPartList READ photoPartList WRITE setPhotoPartList NOTIFY photoPartListChange)
    Q_PROPERTY(int nbPrint READ nbPrint WRITE setNbPrint NOTIFY nbPrintChanged)
    Q_PROPERTY(Template* currentTemplate READ currentTemplate WRITE setCurrentTemplate NOTIFY currentTemplateChanged)
    Q_PROPERTY(bool tweeter READ photoTweeter WRITE setPhotoTweeter NOTIFY tweeterChanged)
    Q_PROPERTY(QString tweeterMessage READ photoTweeterMessage WRITE setPhotoTweeterMessage NOTIFY photoTweeterMessageChanged)
    Q_PROPERTY(QString photoTweeterProfileName READ photoTweeterProfileName WRITE setPhotoTweeterProfileName NOTIFY photoTweeterProfileNameChanged)
    Q_PROPERTY(QString effectName READ effectName WRITE setEffectName NOTIFY effectNameChanged)
public:
    Photo();
    Photo(QString name, Template *t);
    Photo(const Value &value, Template *t);
    ~Photo();

    QUrl finalResult() const;
    void setFinalResult(const QUrl &finalResult);

    QString name() const;
    void setName(const QString &name);

    QList<QObject *> photoPartList() const;
    void setPhotoPartList(const QList<QObject *> &photoPartList);

    int nbPrint() const;
    void setNbPrint(int nbPrint);

    Q_INVOKABLE void addPhotoPart(TemplatePhotoPosition *t);

    Template *currentTemplate() const;
    void setCurrentTemplate(Template *currentTemplate);

    QUrl finalResultSD() const;
    void setFinalResultSD(const QUrl &finalResultSD);

    void Serialize(PrettyWriter<StringBuffer>& writer) const;

    bool photoTweeter() const;
    void setPhotoTweeter(bool photoTweeter);

    QString photoTweeterMessage() const;
    void setPhotoTweeterMessage(const QString &photoTweeterMessage);

    QString photoTweeterProfileName() const;
    void setPhotoTweeterProfileName(const QString &photoTweeterProfileName);

    QString effectName() const;
    void setEffectName(const QString &effectName);

private:
    int                 m_nbPrint;
    QUrl                m_finalResult;
    QUrl                m_finalResultSD;
    QString             m_name;
    QString             m_effectName;
    bool                m_photoTweeter;
    QString             m_photoTweeterMessage;
    QString             m_photoTweeterProfileName;
    QList<QObject*>     m_photoPartList;
    Template*           m_currentTemplate;

    void                Unserialize(Value const &value, Template *t);

signals:
    void finalResultChanged();
    void finalResultSDChanged();
    void nameChanged();
    void nbPrintChanged();
    void currentTemplateChanged();
    void photoPartListChange();
    void tweeterChanged();
    void photoTweeterMessageChanged();
    void photoTweeterProfileNameChanged();
    void tweeterLoadingComplete();
    void effectNameChanged();
};

#endif // PHOTO_H
