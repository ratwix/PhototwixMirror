#include <QQmlEngine>

#include "printermanager.h"

PrinterManager::PrinterManager(QObject *parent) : QObject(parent)
{

}

PrinterManager::PrinterManager(Parameters *parameters)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    m_parameters = parameters;
    m_paperPrint = 0;
    m_updatePaperProcess = new QProcess(this);
    connect(m_updatePaperProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [=](int exitCode, QProcess::ExitStatus exitStatus){ updatePaperPrintFinished();});
}

void PrinterManager::updatePaperPrint()
{
    QString program = "sudo";
    QStringList arguments;
    arguments << m_parameters->getApplicationDirPath().toString() + "/" + SCRIPT_GET_PAPER << "-s";
    m_updatePaperProcess->start(program, arguments);

}

void PrinterManager::printPhoto(QUrl url, bool doubleprint, bool cutprint, bool landscape)
{
    QProcess printProcess;
    QString program = m_parameters->getApplicationDirPath().toString() + "/" + SCRIPT_PRINT;
    QStringList arguments;
    arguments << QString("duplicate:") + (doubleprint ? "true":"false") << QString("portrait:") + (landscape ? "false":"true") << QString("cutter:") + (cutprint?"true":"false") << url.toLocalFile();
    printProcess.start(program, arguments);
    printProcess.waitForFinished(20000);
}

int PrinterManager::paperPrint() const
{
    return m_paperPrint;
}

void PrinterManager::setPaperPrint(int paperPrint)
{
    m_paperPrint = paperPrint;
    emit paperPrintChanged();
}

void PrinterManager::updatePaperPrintFinished()
{
    QString output(m_updatePaperProcess->readAllStandardError());
    QStringList outputSList = output.split("\n", QString::SkipEmptyParts);

    QRegExp paperRx(".*Native Prints Remaining on Media: ([0-9]+)");

    bool find = false;

    for (int i = 0; i < outputSList.length(); i++) {
        QString wl = outputSList.at(i);
        wl = wl.trimmed();
        if (paperRx.exactMatch(wl)) {
            setPaperPrint(paperRx.cap(1).toInt());
            find = true;
            break;
        }
    }

    if (!find) {
        setPaperPrint(0);
    }
}
