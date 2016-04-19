#ifndef MAIL_H
#define MAIL_H

#include <QObject>
#include <QString>
#include <parameters.h>
//#include <vmime/vmime.hpp>


class Mail : public QObject
{
    Q_OBJECT
public:
    explicit Mail(QObject *parent = 0);
    Mail(Parameters *p);

    Parameters *parameters() const;
    void setParameters(Parameters *parameters);

    Q_INVOKABLE void sendMail(QString mail, QString photoPath);
    Q_INVOKABLE void saveMail(QUrl url);
    Q_INVOKABLE void resetMail();
private:
    QString     m_mail_path;
    Parameters *m_parameters;
    //vmime::shared_ptr <vmime::net::session> m_session;

signals:
    void mailSend();
    void mailFailed();
public slots:
};

#endif // MAIL_H
