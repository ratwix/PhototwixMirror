#include "wifimanager.h"

WifiManager::WifiManager(QObject *parent) : QObject(parent)
{

}

WifiManager::WifiManager(Parameters *parameters)
{
    m_parameters = parameters;    
}

void WifiManager::refreshWifiList()
{
    //clean
    /*
    for (QList<WifiItem *>::const_iterator it = m_currentWifiList.begin(); it != m_currentWifiList.end(); it++) {
        delete (*it);
    }
    m_currentWifiList.clear();
    */
    //TODO: Executer la commande iwlist wlan0 scan
    //TODO: parser le résultat et ajouter chaque wifi a la liste m_currentWifiList. Pour chaque wifi, voir si on a pas deja le mot de passe dans m_knownWifi

    createWifiMokup();

    CLog::Write(CLog::Debug, QString("Wifi mokup size") + QString::number(m_currentWifiList.length()));

    emit currentWifiListChanged();
}

void WifiManager::connectWifi(WifiItem *wifi)
{
    //TODO: mettre le setPassword avant d'appeler connectWifi
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

    //Connection au wifi
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

void WifiManager::createWifiMokup()
{
    WifiItem *w1 = new WifiItem();
    w1->setWifiESSID("Wifi1ESSID");
    w1->setWifiAddress("11:11:11:11:11");
    w1->setWifiHasPassword(true);
    w1->setWifiPassword("Wifi1Password");
    w1->setWifiQuality(83);
    m_currentWifiList.append(w1);

    WifiItem *w2 = new WifiItem();
    w2->setWifiESSID("Wifi2ESSID");
    w2->setWifiAddress("22:22:22:22:22");
    w2->setWifiHasPassword(true);
    w2->setWifiPassword("Wifi2Password");
    w2->setWifiQuality(70);
    m_currentWifiList.append(w2);

    WifiItem *w3 = new WifiItem();
    w3->setWifiESSID("Wifi3ESSID");
    w3->setWifiAddress("33:33:33:33:33");
    w3->setWifiHasPassword(true);
    w3->setWifiPassword("Wifi3Password");
    w3->setWifiQuality(45);
    m_currentWifiList.append(w3);
}
