#ifndef EFFECT_H
#define EFFECT_H

#include <QObject>
#include <QUrl>
#include <QList>
#include <string>
#include "clog.h"

#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"

#include "common.h"

using namespace std;
using namespace rapidjson;

class Effect : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString effectName READ getEffectName WRITE setEffectName NOTIFY effectChanged)
    Q_PROPERTY(bool effectEnable READ getEffectEnable WRITE setEffectEnable NOTIFY effectChanged)
    Q_PROPERTY(bool effectTwitterDefault READ getEffectTwitterDefault WRITE setEffectTwitterDefault NOTIFY effectChanged)

public:
    Effect();
    Effect(QString effectName);

    void Serialize(PrettyWriter<StringBuffer>& writer) const;
    void Unserialize(Value const &value);

    QString getEffectName() const;
    void setEffectName(const QString &value);

    bool getEffectEnable() const;
    void setEffectEnable(bool value);

    bool getEffectTwitterDefault() const;
    void setEffectTwitterDefault(bool value);

private:
    QString     m_effectName;
    bool        m_effectEnable;
    bool        m_effectTwitterDefault;

signals:
    void        effectChanged();

public slots:
};

#endif // EFFECT_H
