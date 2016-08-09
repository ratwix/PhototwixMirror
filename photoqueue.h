#ifndef PHOTOQUEUE_H
#define PHOTOQUEUE_H

#include <QObject>
#include "template.h"
#include "QString"

class PhotoQueue
{
public:
    PhotoQueue();
    virtual ~PhotoQueue();

    QString effect() const;
    void setEffect(const QString &effect);

    QString date() const;
    void setDate(const QString &date);

    Template *usedTemplate() const;
    void setUsedTemplate(Template *usedTemplate);

    QString name() const;
    void setName(const QString &name);

private:
    Template    *m_usedTemplate;
    QString     m_effect;
    QString     m_date;
    QString     m_name;
};

#endif // PHOTOQUEUE_H
