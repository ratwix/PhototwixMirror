#include <QProcess>
#include <QRegExp>

#include "wifimanager.h"
#include "common.h"


WifiManager::WifiManager(QObject *parent) : QObject(parent)
{

}

WifiManager::WifiManager(Parameters *parameters)
{
    m_parameters = parameters;
    m_connectedWifi = "";
    m_connectWifiQuality = 0;

    m_connectedWifiCheckProcess = new QProcess(this);
    connect(m_connectedWifiCheckProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [=](int exitCode, QProcess::ExitStatus exitStatus){ checkWifiConnectedTerminate(exitCode, exitStatus);});


    connect(&m_checkWifiConnected, SIGNAL(timeout()), this, SLOT(checkWifiConnected()));
    m_checkWifiConnected.start(8000); //Check wifi connected each 5s

}

void WifiManager::refreshWifiList()
{
    for (QList<QObject *>::const_iterator it = m_currentWifiList.begin(); it != m_currentWifiList.end(); it++) {
        delete (*it);
    }
    m_currentWifiList.clear();

    QString program = "sudo";
    QStringList arguments;
    arguments << "iwlist" << "wlan0" << "scan";
    QProcess wifiProcess;
    wifiProcess.start(program, arguments);
    wifiProcess.waitForFinished(10000);
    QString output(wifiProcess.readAllStandardOutput());

    QStringList outputSList = output.split("\n", QString::SkipEmptyParts);

    QRegExp newWifiRx("Cell\\s[0-9]{2}.*Address:\\s(.*)");
    QRegExp qualityRx("Quality=([0-9]+)/([0-9]+).*");
    QRegExp hasPasswordRx("Encryption key:(on|off)");
    QRegExp essidRx("ESSID:\"(.*)\"");

    //Rafraichit la liste des wifis actuels
    WifiItem *wifi;

    for (int i = 0; i < outputSList.length(); i++) {
        QString wl = outputSList.at(i);
        wl = wl.trimmed();

        if (newWifiRx.exactMatch(wl)) { //Nouveau wifi
            CLog::Write(CLog::Debug, QString("Wifi address:") + newWifiRx.cap(1));
            wifi = new WifiItem();
            QQmlEngine::setObjectOwnership(wifi, QQmlEngine::CppOwnership);
            m_currentWifiList.append(wifi);
            wifi->setWifiAddress(newWifiRx.cap(1).trimmed());
            continue;
        }
        if (qualityRx.exactMatch(wl)) { //Qualité du wifi
            int aq = qualityRx.cap(1).toInt();
            int bq = qualityRx.cap(2).toInt();
            int quality = bq > 0 ? aq * 100 / bq : 0;
            wifi->setWifiQuality(quality);
            CLog::Write(CLog::Debug, QString("Wifi quality:") + QString::number(quality));
            continue;
        }
        if (hasPasswordRx.exactMatch(wl)) { //Wifi ouvert
            if (hasPasswordRx.cap(1) == "on") {
                wifi->setWifiHasPassword(true);
            } else {
                wifi->setWifiHasPassword(false);
            }
            CLog::Write(CLog::Debug, QString("Wifi has password:") + QString::number(wifi->wifiHasPassword()));
        }
        if (essidRx.exactMatch(wl)) { //Wifi essid
            wifi->setWifiESSID(essidRx.cap(1));
            CLog::Write(CLog::Debug, QString("Wifi ESSID:") + wifi->wifiESSID());
            //Ajout du mot de passe si il est déjà connu
            for (QList<QObject *>::const_iterator it = m_knownWifi.begin(); it != m_knownWifi.end(); it++) {
                if (WifiItem *item = dynamic_cast<WifiItem *>(*it)) {
                    if (item->wifiESSID() == wifi->wifiESSID()) {    //c'est le même wifi
                        wifi->setWifiPassword(item->wifiPassword()); //met le wifi a jour
                        break;
                    }
                }
            }
        }
    }
    emit currentWifiListChanged();
}

void WifiManager::connectWifi(WifiItem *wifi)
{
    //Mise a jout de la base de wifi connue
    bool find = false;
    for (QList<QObject *>::const_iterator it = m_knownWifi.begin(); it != m_knownWifi.end(); it++) {
        if (WifiItem *item = dynamic_cast<WifiItem *>(*it)) {
            if (item->wifiESSID() == wifi->wifiESSID()) {    //c'est le même wifi
                item->setWifiPassword(wifi->wifiPassword()); //Change the password
                find = true;
            }
        }
    }

    if (!find) {
        m_knownWifi.append(wifi);
    }

    m_parameters->Serialize();

    //Affichage d'un message

    emit wifiTryConnect(wifi->wifiESSID());

    //Connection au wifi
    QString program = "sudo";
    QStringList arguments;
    arguments << m_parameters->getApplicationDirPath().toString() + "/" + SCRIPT_CONNECT_WIFI << wifi->wifiESSID() << wifi->wifiPassword();
    QProcess wifiConnectProcess;
    wifiConnectProcess.start(program, arguments);
    wifiConnectProcess.waitForFinished(10000);
}

void WifiManager::serialize(PrettyWriter<StringBuffer> &writer) const
{
    for (QList<QObject *>::const_iterator it = m_knownWifi.begin(); it != m_knownWifi.end(); it++) {
        if (WifiItem *item = dynamic_cast<WifiItem *>(*it)) {
            writer.StartObject();

            writer.Key("wifiESSID");
            writer.String(item->wifiESSID().toStdString().c_str());

            writer.Key("wifiAddress");
            writer.String(item->wifiAddress().toStdString().c_str());

            writer.Key("wifiPassword");
            writer.String(item->wifiPassword().toStdString().c_str());

            writer.Key("wifiHasPassword");
            writer.Bool(item->wifiHasPassword());

            writer.EndObject();
        }
    }
}

void WifiManager::unserialize(const Value &values)
{
    //clean m_knownWifi
    for (QList<QObject *>::const_iterator it = m_knownWifi.begin(); it != m_knownWifi.end(); it++) {
        delete (*it);
    }
    m_knownWifi.clear();

    for (SizeType i = 0; i < values.Size(); i++) {
        Value const &value = values[i];
        WifiItem *wi = new WifiItem();
        QQmlEngine::setObjectOwnership(wi, QQmlEngine::CppOwnership);
        if (value.HasMember("wifiESSID")) {
            wi->setWifiESSID(QString(value["wifiESSID"].GetString()));
        }

        if (value.HasMember("wifiAddress")) {
            wi->setWifiAddress(QString(value["wifiAddress"].GetString()));
        }

        if (value.HasMember("wifiPassword")) {
            wi->setWifiPassword(QString(value["wifiPassword"].GetString()));
        }

        if (value.HasMember("wifiHasPassword")) {
            wi->setWifiHasPassword(value["wifiHasPassword"].GetBool());
        }
        m_knownWifi.append(wi);
    }
}

WifiItem *WifiManager::getCurrentWifi() const
{
    return m_currentWifi;
}

void WifiManager::setCurrentWifi(WifiItem *value)
{
    m_currentWifi = value;
}

QList<QObject *> WifiManager::getKnownWifi() const
{
    return m_knownWifi;
}

void WifiManager::setKnownWifi(const QList<QObject *> &value)
{
    m_knownWifi = value;
}

QList<QObject *> WifiManager::getCurrentWifiList() const
{
    return m_currentWifiList;
}

void WifiManager::setCurrentWifiList(const QList<QObject *> &value)
{
    m_currentWifiList = value;
}

QString WifiManager::getConnectedWifi() const
{
    return m_connectedWifi;
}

void WifiManager::setConnectedWifi(const QString &connectedWifi)
{
    m_connectedWifi = connectedWifi;
    emit connectedWifiChanged();
}

int WifiManager::getConnectWifiQuality() const
{
    return m_connectWifiQuality;
}

void WifiManager::setConnectWifiQuality(int connectWifiQuality)
{
    m_connectWifiQuality = connectWifiQuality;
    emit connectWifiQualityChanged();
}

void WifiManager::checkWifiConnectedTerminate(int exitCode, QProcess::ExitStatus exitStatus)
{
    bool        connected = false;
    QString     ssid = "";
    int         quality = 0;
    QString output(m_connectedWifiCheckProcess->readAllStandardOutput());
    QStringList sl = output.split("\n", QString::SkipEmptyParts);

    QRegExp connectedRx("Connected to.*");
    QRegExp signalRx("signal: -([0-9]+).*");
    QRegExp ssidRx("SSID: (.*)");

    for (int i = 0; i < sl.length(); i++) {
        QString s = sl.at(i);
        s = s.trimmed();
        if (connectedRx.exactMatch(s)) { //On est connecté au wifi
            connected = true;
            continue;
        }
        if (signalRx.exactMatch(s)) {
            if (connected) {
                quality = 2 * (signalRx.cap(1).toInt() * -1 + 100);
                setConnectWifiQuality(quality);
            }
            continue;
        }
        if (ssidRx.exactMatch(s)) {
            setConnectedWifi(ssidRx.cap(1));
        }
    }

    if (!connected) {
        setConnectWifiQuality(0);
        setConnectedWifi("");

    }
}

void WifiManager::checkWifiConnected()
{
    if (m_connectedWifiCheckProcess->state() == QProcess::Running) {
        m_connectedWifiCheckProcess->terminate();
    }
    QString program = "iw";
    QStringList arguments;
    arguments << "wlan0" << "link";
    m_connectedWifiCheckProcess->start(program, arguments);
}
