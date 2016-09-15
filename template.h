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
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY templateChanged)
    Q_PROPERTY(QUrl url READ getUrl WRITE setUrl NOTIFY templateChanged)
    Q_PROPERTY(bool active READ getActive WRITE setActive NOTIFY templateChanged)
    Q_PROPERTY(QList<QObject*> templatePhotoPositions READ templatePhotoPositions WRITE setTemplatePhotoPositions NOTIFY templateChanged)
    Q_PROPERTY(bool printcutter READ getPrintcutter WRITE setPrintcutter NOTIFY templateChanged)
    Q_PROPERTY(bool doubleprint READ getDoubleprint WRITE setDoubleprint NOTIFY templateChanged)
    Q_PROPERTY(bool landscape READ getLandscape WRITE setLandscape NOTIFY templateChanged)
    Q_PROPERTY(bool twitterDefault READ getTwitterDefault WRITE setTwitterDefault NOTIFY templateChanged)
    Q_PROPERTY(QString twitterMessageColor READ getTwitterMessageColor WRITE setTwitterMessageColor NOTIFY twitterMessageColorChanged)

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
    Q_INVOKABLE int  nbPhotos();

    bool getPrintcutter() const;
    void setPrintcutter(bool printcutter);

    bool getDoubleprint() const;
    void setDoubleprint(bool doubleprint);

    bool getLandscape() const;
    void setLandscape(bool landscape);

    bool getTwitterDefault() const;
    void setTwitterDefault(bool twitterDefault);

    QString getTwitterMessageColor() const;
    void setTwitterMessageColor(const QString &twitterMessageColor);

signals:
    void templateChanged();
    void activeChanged();
    void twitterMessageColorChanged();

private:
    bool            m_active;
    QString         m_name;
    QUrl            m_url;
    QList<QObject*> m_templatePhotoPositions;
    Parameters      *m_parameters;
    bool            m_printcutter;
    bool            m_doubleprint;
    bool            m_landscape;
    bool            m_twitterDefault;
    QString         m_twitterMessageColor;
};

#endif // TEMPLATE_H
