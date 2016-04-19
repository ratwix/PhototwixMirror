#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <QObject>
#include <QUrl>
#include <QList>
#include <string>
#include "clog.h"

#include "common.h"

#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"
#include "parameters.h"
#include "templatephotoposition.h"

using namespace std;
using namespace rapidjson;

class Parameters;
class Template : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QUrl url READ getUrl WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(bool active READ getActive WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(QList<QObject*> templatePhotoPositions READ templatePhotoPositions WRITE setTemplatePhotoPositions NOTIFY templatePhotoPositionsChanged)
    Q_PROPERTY(bool printcutter READ getPrintcutter WRITE setPrintcutter NOTIFY printcutterChanged)
    Q_PROPERTY(bool doubleprint READ getDoubleprint WRITE setDoubleprint NOTIFY doubleprintChanged)
    Q_PROPERTY(bool landscape READ getLandscape WRITE setLandscape NOTIFY landscapeChanged)

public:
    Template();
    Template(QString name, Parameters *parameters);
    Template(Value const &value, Parameters *parameters);
    ~Template();

    //Property Accesseurs
    QString getName() const;
    void setName(QString name);

    QUrl getUrl() const;
    void setUrl(QUrl url);

    bool getActive() const;
    void setActive(bool active);

    QList<QObject *> templatePhotoPositions() const;
    void setTemplatePhotoPositions(const QList<QObject *> &templatePhotoPositions);
    //End of accessors

    void Serialize(PrettyWriter<StringBuffer>& writer) const;
    void Unserialize(Value const &value);
    void addTemplatePhotoPosition(Value const &value);

    Q_INVOKABLE void addTemplatePhotoPosition();
    Q_INVOKABLE void deleteTemplatePhotoPosition(int i);
    Q_INVOKABLE void updateImageFromUrl(QUrl url);

    bool getPrintcutter() const;
    void setPrintcutter(bool printcutter);

    bool getDoubleprint() const;
    void setDoubleprint(bool doubleprint);

    bool getLandscape() const;
    void setLandscape(bool landscape);

signals:
    void nameChanged(QString);
    void urlChanged();
    void activeChanged();
    void templatePhotoPositionsChanged();
    void printcutterChanged();
    void doubleprintChanged();
    void landscapeChanged();

private:
    bool            m_active;
    QString         m_name;
    QUrl            m_url;
    QList<QObject*> m_templatePhotoPositions;
    Parameters      *m_parameters;
    bool            m_printcutter;
    bool            m_doubleprint;
    bool            m_landscape;
};

#endif // TEMPLATE_H
