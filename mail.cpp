#include <QFileInfo>
#include <QDate>
#include "common.h"
#include "mail.h"
#include "clog.h"


Mail::Mail(QObject *parent) : QObject(parent)
{

}

Mail::Mail(Parameters *p)
{
    m_parameters = p;
    m_mail_path = m_parameters->getApplicationDirPath().toString() + "/contact_mails.csv";
/*
    m_session = vmime::make_shared <vmime::net::session>();
*/
}

Parameters *Mail::parameters() const
{
    return m_parameters;
}

void Mail::setParameters(Parameters *parameters)
{
    m_parameters = parameters;
}

void Mail::sendMail(QString mail, QString photoPath)
{
    //Write address to file
    try {
        QFile file(m_mail_path);
        if ( file.open(QIODevice::Append) )
        {
            QTextStream stream( &file );
            stream <<  QDateTime::currentDateTime().toString("dd.MM.yyyy_hh:mm:ss") << ";" << mail << ";" << QFileInfo(photoPath).fileName() << endl;
        }
        file.close();
    } catch (std::exception& e) {
        CLog::Write(CLog::Error, e.what());
    }

/*
    try {

        //CLog::Write(CLog::Debug, "Envoie du mail" + mail.toStdString() + " photo:" + photoPath.toStdString());

         vmime::utility::url url("smtp://" + m_parameters->getMailSmtp().toStdString() + ":" + m_parameters->getMailPort().toStdString());
         vmime::shared_ptr <vmime::net::transport> tr = m_session->getTransport(url);
         tr->setProperty("options.need-authentication", true);
         tr->setProperty("auth.username", m_parameters->getMailUsername().toStdString());
         tr->setProperty("auth.password", m_parameters->getMailPassword().toStdString());

         vmime::messageBuilder mb;

         // Fill in the basic fields
         mb.setExpeditor(vmime::mailbox(m_parameters->getMailFrom().toStdString()));

         vmime::addressList to;
         to.appendAddress(vmime::make_shared <vmime::mailbox>(mail.toStdString()));

         mb.setRecipients(to);

         if (m_parameters->getMailBcc() != "") {
             vmime::addressList bcc;
             bcc.appendAddress(vmime::make_shared <vmime::mailbox>(m_parameters->getMailBcc().toStdString()));

             mb.setBlindCopyRecipients(bcc);
         }

         mb.setSubject(vmime::text(m_parameters->getMailSubject().toStdString()));

         // Message body
         mb.getTextPart()->setText(vmime::make_shared <vmime::stringContentHandler>(
             m_parameters->getMailContent().toStdString()));

         // Adding an attachment
         if (QFileInfo(photoPath).exists()) {
             vmime::shared_ptr <vmime::fileAttachment> a = vmime::make_shared <vmime::fileAttachment>
             (
                 photoPath.toStdString(),                        // full path to file
                 vmime::mediaType("application/octet-stream"),   // content type
                 vmime::text("Photo")              // description
             );

             a->getFileInfo().setFilename(QFileInfo(photoPath).fileName().toStdString());
             a->getFileInfo().setCreationDate(vmime::datetime().now());
             mb.attach(a);
         }

         // Construction
         vmime::shared_ptr <vmime::message> msg = mb.construct();

         tr->connect();
         tr->send(msg);
         tr->disconnect();
         emit mailSend();

    }

    catch (vmime::exception& e)
    {
        std::cerr << std::endl;
        std::cerr << e.what() << std::endl;
        emit mailFailed();
    }
    catch (std::exception& e)
    {
        std::cerr << std::endl;
        std::cerr << "std::exception: " << e.what() << std::endl;
        emit mailFailed();
    }
*/
}

void Mail::saveMail(QUrl url)
{
    QFile file(m_mail_path);
    if (file.exists()) {
        file.copy(url.toString() + "/contact_mail.csv");
    }
}

void Mail::resetMail()
{
    try {
        QFile file(m_mail_path);
        if (file.exists()) {
            file.remove();
        }

    } catch (std::exception& e) {
        CLog::Write(CLog::Error, e.what());
    }
}

