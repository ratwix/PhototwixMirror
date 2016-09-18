#include <QQmlEngine>
#include <sstream>
#include <string>
#include "photo.h"

Photo::Photo():
    m_finalResult(""),
    m_finalResultSD(""),
    m_nbPrint(0),
    m_photoTweeter(false),
    m_photoTweeterMessage(""),
    m_photoTweeterProfileName(""),
    m_effectName("")
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

Photo::Photo(QString name, Template *t):
    m_finalResult(""),
    m_finalResultSD(""),
    m_nbPrint(0),
    m_photoTweeter(false),
    m_photoTweeterMessage(""),
    m_photoTweeterProfileName(""),
    m_effectName("")
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    m_name = name;
    m_currentTemplate = t;

    //For all photo template position, add a new photopart

    QList<QObject*>::iterator it;

    if (t) {
        QList<QObject *> tppList = t->templatePhotoPositions();

        for (it = tppList.begin(); it != tppList.end(); it++) {
            QObject *tppo = (*it);
            if (TemplatePhotoPosition* tpp = dynamic_cast<TemplatePhotoPosition*>(tppo)) {
                addPhotoPart(tpp);
            } else {
                CLog::Write(CLog::Warning, "Unable to cast TemplatePhotoPosition");
            }

        }
    }
}

Photo::Photo(const Value &value, Template *t):
    m_finalResult(""),
    m_finalResultSD(""),
    m_nbPrint(0),
    m_photoTweeter(false),
    m_photoTweeterMessage(""),
    m_photoTweeterProfileName("")
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    m_currentTemplate = t;
    Unserialize(value, t);
}

Photo::~Photo()
{
    CLog::Write(CLog::Debug, "Delete Photo " + m_finalResult.toString().toStdString());

    QList<QObject*>::iterator it;

    for (it = m_photoPartList.begin(); it != m_photoPartList.end(); it++) {
        delete *it;
    }
}

QUrl Photo::finalResult() const
{
    return m_finalResult;
}

void Photo::setFinalResult(const QUrl &finalResult)
{
    m_finalResult = finalResult;
    emit finalResultChanged();
}

QString Photo::name() const
{
    return m_name;
}

void Photo::setName(const QString &name)
{
    m_name = name;
}

QList<QObject *> Photo::photoPartList() const
{
    ostringstream test;
    test << "Get photo part list :" << m_photoPartList.count() << ":";
    //CLog::Write(CLog::Debug, test.str());
    return m_photoPartList;
}

void Photo::setPhotoPartList(const QList<QObject *> &photoPartList)
{
    m_photoPartList = photoPartList;
}

int Photo::nbPrint() const
{
    return m_nbPrint;
}

void Photo::setNbPrint(int nbPrint)
{
    m_nbPrint = nbPrint;
}

Template *Photo::currentTemplate() const
{
    return m_currentTemplate;
}

void Photo::setCurrentTemplate(Template *currentTemplate)
{
    m_currentTemplate = currentTemplate;
}
QUrl Photo::finalResultSD() const
{
    return m_finalResultSD;
}

void Photo::setFinalResultSD(const QUrl &finalResultSD)
{
    m_finalResultSD = finalResultSD;
    emit finalResultSDChanged();
}

void Photo::Serialize(PrettyWriter<StringBuffer> &writer) const
{
    writer.StartObject();

    writer.Key("finalResult");
    writer.String(m_finalResult.toString().toStdString().c_str());

    writer.Key("finalResultSD");
    writer.String(m_finalResultSD.toString().toStdString().c_str());

    writer.Key("name");
    writer.String(m_name.toStdString().c_str());

    writer.Key("nbPrint");
    writer.Int(m_nbPrint);

    writer.Key("currentTemplate");
    writer.String(m_currentTemplate->getName().toStdString().c_str());

    writer.Key("photoTweeter");
    writer.Bool(m_photoTweeter);

    writer.Key("photoTweeterMessage");
    writer.String(m_photoTweeterMessage.toStdString().c_str());

    writer.Key("photoTweeterProfileName");
    writer.String(m_photoTweeterProfileName.toStdString().c_str());

    writer.Key("photoEffectName");
    writer.String(m_effectName.toStdString().c_str());

    QList<QObject*>::const_iterator it;
    writer.Key("photosPart");
    writer.StartArray();

    for (it = m_photoPartList.begin(); it != m_photoPartList.end(); it++) {
        PhotoPart *pp = dynamic_cast<PhotoPart*>(*it);
        if (pp != 0) {
            pp->Serialize(writer);
        }
    }

    writer.EndArray();

    writer.EndObject();
}

bool Photo::photoTweeter() const
{
    return m_photoTweeter;
}

void Photo::setPhotoTweeter(bool photoTweeter)
{
    m_photoTweeter = photoTweeter;
}

QString Photo::photoTweeterMessage() const
{
    return m_photoTweeterMessage;
}

void Photo::setPhotoTweeterMessage(const QString &photoTweeterMessage)
{
    m_photoTweeterMessage = photoTweeterMessage;
}

QString Photo::photoTweeterProfileName() const
{
    return m_photoTweeterProfileName;
}

void Photo::setPhotoTweeterProfileName(const QString &photoTweeterProfileName)
{
    m_photoTweeterProfileName = photoTweeterProfileName;
}

QString Photo::effectName() const
{
    return m_effectName;
}

void Photo::setEffectName(const QString &effectName)
{
    m_effectName = effectName;
}

void Photo::Unserialize(const Value &value, Template *t)
{
    if (value.HasMember("finalResult")) {
        m_finalResult = QString(value["finalResult"].GetString());
    }
    
    if (value.HasMember("finalResultSD")) {
        m_finalResultSD = QString(value["finalResultSD"].GetString());
    }

    if (value.HasMember("name")) {
        m_name = QString(value["name"].GetString());
    }

    if (value.HasMember("nbPrint")) {
        m_nbPrint = value["nbPrint"].GetInt();
    }

    if (value.HasMember("photoTweeter")) {
        m_photoTweeter = value["photoTweeter"].GetBool();
    }

    if (value.HasMember("photoTweeterMessage")) {
        m_photoTweeterMessage = QString(value["photoTweeterMessage"].GetString());
    }

    if (value.HasMember("photoEffectName")) {
        m_effectName = QString(value["photoEffectName"].GetString());
    }

    if (value.HasMember("photoTweeterProfileName")) {
        m_photoTweeterProfileName = QString(value["photoTweeterProfileName"].GetString());
    }

    //Add all photo part
    if (t) {
        QList<QObject *> tppList = t->templatePhotoPositions();
        for (QList<QObject *>::iterator it = tppList.begin(); it != tppList.end(); it++) {
            QObject *tppo = (*it);
            if (TemplatePhotoPosition* tpp = dynamic_cast<TemplatePhotoPosition*>(tppo)) {
                addPhotoPart(tpp);
            } else {
                CLog::Write(CLog::Warning, "Unable to cast TemplatePhotoPosition");
            }

        }
    }

    //Add all photo part URL
    if (value.HasMember("photosPart")) {
        const Value& photoPart = value["photosPart"];
        if (photoPart.IsArray()) {
            for (SizeType i = 0; i < photoPart.Size(); i++) {
                if (i <= (SizeType) m_photoPartList.length()) {
                    if (PhotoPart *pp = dynamic_cast<PhotoPart *>(m_photoPartList[i])) {
                        if (photoPart[i].HasMember("photoPartUrl")) {
                            string url = photoPart[i]["photoPartUrl"].GetString();
                            pp->setPath(QUrl(QString(url.c_str())));
                        }
                    }
                }
            }
        }
    }
}

void Photo::addPhotoPart(TemplatePhotoPosition *t)
{
    PhotoPart* pp = new PhotoPart(t);
    QQmlEngine::setObjectOwnership(pp, QQmlEngine::CppOwnership);
    m_photoPartList.append(pp);
}






