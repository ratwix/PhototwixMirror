#include "effect.h"

Effect::Effect()
{
    m_effectName = "";
    m_effectEnable = true;
    m_effectTwitterDefault = false;
}

Effect::Effect(QString effectName)
{
    m_effectName = effectName;
    m_effectEnable = true;
    m_effectTwitterDefault = false;
}

void Effect::Serialize(PrettyWriter<StringBuffer> &writer, Parameters *p) const
{
    p->updateEffect(m_effectName, m_effectEnable, m_effectTwitterDefault);

    writer.StartObject();

    writer.Key("effect_name");
    writer.String(m_effectName.toStdString().c_str());

    writer.Key("effect_enable");
    writer.Bool(m_effectEnable);

    writer.Key("effect_twitterDefault");
    writer.Bool(m_effectTwitterDefault);

    writer.EndObject();
}

void Effect::Unserialize(const Value &value)
{
    if (value.HasMember("effect_name")) {
        m_effectName = QString(value["effect_name"].GetString());
    }

    if (value.HasMember("effect_enable")) {
        m_effectEnable = value["effect_enable"].GetBool();
    }

    if (value.HasMember("effect_twitterDefault")) {
        m_effectTwitterDefault = value["effect_twitterDefault"].GetBool();
    }
}

QString Effect::getEffectName() const
{
    return m_effectName;
}

void Effect::setEffectName(const QString &value)
{
    m_effectName = value;
}

bool Effect::getEffectEnable() const
{
    return m_effectEnable;
}

void Effect::setEffectEnable(bool value)
{
    m_effectEnable = value;
}

bool Effect::getEffectTwitterDefault() const
{
    return m_effectTwitterDefault;
}

void Effect::setEffectTwitterDefault(bool value)
{
    m_effectTwitterDefault = value;
}
