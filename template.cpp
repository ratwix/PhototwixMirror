#include <QApplication>
#include <sstream>
#include <QQmlEngine>
#include <QFile>
#include "template.h"

Template::Template()
{
    m_parameters = 0;
    setName("error");
    m_active = false;
    m_printcutter = false;
    m_doubleprint = false;
    m_landscape = true;
}

Template::Template(QString name, Parameters *parameters)
{
    m_parameters = parameters;
    setName(name);
    m_active = false;
    m_printcutter = false;
    m_doubleprint = false;
    m_landscape = true;
    QString path = QString("file:///") + parameters->getApplicationDirPath().toString() + "/" + TEMPLATE_FOLDER + "/" + name;

    setUrl(QUrl(path));
}

Template::Template(Value const &value, Parameters *parameters) {
    m_parameters = parameters;
    Unserialize(value);
}

Template::~Template()
{
    CLog::Write(CLog::Debug, "Delete Template");
    //Free templatesPhotoPosition
    QList<QObject*>::iterator it;

    for (it = m_templatePhotoPositions.begin(); it != m_templatePhotoPositions.end(); it++) {
        delete *it;
    }
}

QString Template::getName() const {
    return m_name;
}

void Template::setName(QString name) {
    m_name = name;
}

QUrl Template::getUrl() const {
    return m_url;
}

void Template::setUrl(QUrl url)  {
    m_url = url;
}

bool Template::getActive() const {
    return m_active;
}

void Template::setActive(bool active) {
    if (active != m_active) {
        this->m_active = active;

        CLog::Write(CLog::Info, m_name.toStdString() + " active:" + (this->m_active ? "true" : "false"));
        //Save change to json files
        m_parameters->Serialize();
        emit activeChanged();
        m_parameters->rebuildActivesTemplates();
    }
}

QList<QObject *> Template::templatePhotoPositions() const
{
    return m_templatePhotoPositions;
}

void Template::setTemplatePhotoPositions(const QList<QObject *> &templatePhotoPositions)
{
    m_templatePhotoPositions = templatePhotoPositions;
}

void Template::Serialize(PrettyWriter<StringBuffer> &writer) const {
    writer.StartObject();

    writer.Key("template_name");
    writer.String(m_name.toStdString().c_str());

    writer.Key("url");
    writer.String(m_url.toString().toStdString().c_str());

    writer.Key("active");
    writer.Bool(m_active);

    writer.Key("printcutter");
    writer.Bool(m_printcutter);

    writer.Key("doubleprint");
    writer.Bool(m_doubleprint);

    writer.Key("landscape");
    writer.Bool(m_landscape);

    //Serialisation des TemplatePhotoPosition
    QList<QObject*>::const_iterator it;

    writer.Key("templatesPhotoPositions");
    writer.StartArray();

    for (it = m_templatePhotoPositions.begin(); it != m_templatePhotoPositions.end(); it++) {
        TemplatePhotoPosition *tpp = dynamic_cast<TemplatePhotoPosition*>(*it);
        if (tpp != 0) {
            tpp->Serialize(writer);
        }
    }

    writer.EndArray();

    writer.EndObject();
}


void Template::Unserialize(Value const &value) {
    if (value.HasMember("template_name")) {
        m_name = QString(value["template_name"].GetString());
    }

    if (value.HasMember("url")) {
        m_url = QUrl(value["url"].GetString());
    }

    if (value.HasMember("active")) {
        m_active = value["active"].GetBool();
    }

    if (value.HasMember("printcutter")) {
        m_printcutter = value["printcutter"].GetBool();
    }

    if (value.HasMember("doubleprint")) {
        m_doubleprint = value["doubleprint"].GetBool();
    }

    if (value.HasMember("landscape")) {
        m_landscape = value["landscape"].GetBool();
    }

    if (value.HasMember("templatesPhotoPositions")) {
        const Value& templatesPhotoPositions = value["templatesPhotoPositions"];
        if (templatesPhotoPositions.IsArray()) {
            for (SizeType i = 0; i < templatesPhotoPositions.Size(); i++) {
                addTemplatePhotoPosition(templatesPhotoPositions[i]);
            }
        }
    }
}

void Template::addTemplatePhotoPosition(const Value &value)
{
    std::stringstream sstm;
    sstm << "Unserialise new Template Photo Position : " << m_templatePhotoPositions.count() << " for template " << m_name.toStdString();
    CLog::Write(CLog::Info, sstm.str());

    TemplatePhotoPosition *t = new TemplatePhotoPosition(value);
    QQmlEngine::setObjectOwnership(t, QQmlEngine::CppOwnership);
    m_templatePhotoPositions.append(t);
    t->setNumber(m_templatePhotoPositions.count());
}

void Template::addTemplatePhotoPosition() {
    std::stringstream sstm;
    sstm << "Add a new Template Photo Position : " << m_templatePhotoPositions.count() << " for template " << m_name.toStdString();
    CLog::Write(CLog::Info, sstm.str());

    TemplatePhotoPosition *t = new TemplatePhotoPosition();
    QQmlEngine::setObjectOwnership(t, QQmlEngine::CppOwnership);
    m_templatePhotoPositions.append(t);

    t->setNumber(m_templatePhotoPositions.count());
}

void Template::deleteTemplatePhotoPosition(int i)
{
    std::stringstream sstm;
    sstm << "Delete Template Photo Position at position : " << i << " for template " << m_name.toStdString();
    CLog::Write(CLog::Info, sstm.str());

    m_templatePhotoPositions.removeAt(i - 1);

    QList<QObject*>::iterator it;
    i = 1;

    for (it = m_templatePhotoPositions.begin(); it != m_templatePhotoPositions.end(); it++) {
        TemplatePhotoPosition *tpp = dynamic_cast<TemplatePhotoPosition*>(*it);

        if (tpp != 0) {
            tpp->setNumber(i++);
        }
    }
}

void Template::updateImageFromUrl(QUrl source_url)
{
    QString source_url_s = source_url.toString();
    QString targer_url_s = m_url.toString();

    CLog::Write(CLog::Info, QString("Change " + source_url_s + " to " + targer_url_s).toStdString());
    //TOTO: change with file path
    if (source_url_s.startsWith("file:////")) {
        source_url_s = source_url_s.right(source_url_s.length() - QString("file:///").length());
    }


    if (source_url_s.startsWith("file:///")) {
        source_url_s = source_url_s.right(source_url_s.length() - QString("file://").length());
    }

    if (targer_url_s.startsWith("file:////")) {
        targer_url_s = targer_url_s.right(targer_url_s.length() - QString("file:///").length());
    }


    if (targer_url_s.startsWith("file:///")) {
        targer_url_s = targer_url_s.right(targer_url_s.length() - QString("file://").length());
    }

    if (QFile::exists(targer_url_s) && QFile::exists(source_url_s)) {
        CLog::Write(CLog::Info, "Success change 1");
        if (QFile::remove(targer_url_s)) {
            CLog::Write(CLog::Info, "Success change 2");
            if (QFile::copy(source_url_s, targer_url_s)) {
                CLog::Write(CLog::Info, "Success change 3");
            }
        }
    } else {
        CLog::Write(CLog::Info, "error");
    }

    emit urlChanged();
}


bool Template::getPrintcutter() const
{
    return m_printcutter;
}

void Template::setPrintcutter(bool printcutter)
{
    m_printcutter = printcutter;
    m_parameters->Serialize();
    emit printcutterChanged();
}
bool Template::getDoubleprint() const
{
    return m_doubleprint;
}

void Template::setDoubleprint(bool doubleprint)
{
    m_doubleprint = doubleprint;
    m_parameters->Serialize();
    emit doubleprintChanged();
}
bool Template::getLandscape() const
{
    return m_landscape;
}

void Template::setLandscape(bool landscape)
{
    m_landscape = landscape;
    m_parameters->Serialize();
    emit landscapeChanged();
}



