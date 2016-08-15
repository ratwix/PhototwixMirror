#ifndef PRINTERMANAGER_H
#define PRINTERMANAGER_H

#include <QObject>
#include <QProcess>
#include "common.h"
#include "parameters.h"

class PrinterManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int paperprint READ paperPrint WRITE setPaperPrint NOTIFY paperPrintChanged)
public:
    explicit PrinterManager(QObject *parent = 0);

    PrinterManager(Parameters *parameters);

    Q_INVOKABLE void updatePaperPrint();
    Q_INVOKABLE void printPhoto(QUrl url, bool doubleprint, bool cutprint, bool landscape);

    int paperPrint() const;
    void setPaperPrint(int paperPrint);

private:
    Parameters  *m_parameters;
    int         m_paperPrint;
    QProcess *  m_updatePaperProcess;

signals:
    void        paperPrintChanged();
public slots:
    void        updatePaperPrintFinished();
};

#endif // PRINTERMANAGER_H
