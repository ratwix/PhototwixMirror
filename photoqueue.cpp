#include "photoqueue.h"

PhotoQueue::PhotoQueue() :
    m_effect(""),
    m_date(""),
    m_usedTemplate(NULL)
{

}

PhotoQueue::~PhotoQueue()
{

}

QString PhotoQueue::effect() const
{
    return m_effect;
}

void PhotoQueue::setEffect(const QString &effect)
{
    m_effect = effect;
}

QString PhotoQueue::date() const
{
    return m_date;
}

void PhotoQueue::setDate(const QString &date)
{
    m_date = date;
}

Template *PhotoQueue::usedTemplate() const
{
    return m_usedTemplate;
}

void PhotoQueue::setUsedTemplate(Template *usedTemplate)
{
    m_usedTemplate = usedTemplate;
}

QString PhotoQueue::name() const
{
    return m_name;
}

void PhotoQueue::setName(const QString &name)
{
    m_name = name;
}
