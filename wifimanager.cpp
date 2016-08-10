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
    for (QList<WifiItem *>::const_iterator it = m_currentWifiList.begin(); it != m_currentWifiList.end(); it++) {
        delete (*it);
    }
    m_currentWifiList.clear();

    //TODO: Executer la commande iwlist wlan0 scan
    //TODO: parser le résultat et ajouter chaque wifi a la liste m_currentWifiList. Pour chaque wifi, voir si on a pas deja le mot de passe dans m_knownWifi

    emit currentWifiListChanged();
}

void WifiManager::connectWifi(WifiItem *wifi)
{
    //TODO: mettre le setPassword avant d'appeler connectWifi
    //Mise a jout de la base de wifi connue
    bool find = false;
    for (QList<WifiItem *>::const_iterator it = m_knownWifi.begin(); it != m_knownWifi.end(); it++) {
        if ((*it)->wifiESSID() == wifi->wifiESSID()) {    //c'est le même wifi
            (*it)->setWifiPassword(wifi->wifiPassword()); //Change the password
            find = true;
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
    for (QList<WifiItem *>::const_iterator it = m_knownWifi.begin(); it != m_knownWifi.end(); it++) {
        writer.StartObject();

        writer.Key("wifiESSID");
        writer.String((*it)->wifiESSID().toStdString().c_str());

        writer.Key("wifiAddress");
        writer.String((*it)->wifiAddress().toStdString().c_str());

        writer.Key("wifiPassword");
        writer.String((*it)->wifiPassword().toStdString().c_str());

        writer.Key("wifiHasPassword");
        writer.Bool((*it)->wifiHasPassword());

        writer.EndObject();
    }
}

void WifiManager::unserialize(const Value &values)
{
    //clean m_knownWifi
    for (QList<WifiItem *>::const_iterator it = m_knownWifi.begin(); it != m_knownWifi.end(); it++) {
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

QList<WifiItem *> WifiManager::getKnownWifi() const
{
    return m_knownWifi;
}

void WifiManager::setKnownWifi(const QList<WifiItem *> &value)
{
    m_knownWifi = value;
}

QList<WifiItem *> WifiManager::getCurrentWifiList() const
{
    return m_currentWifiList;
}

void WifiManager::setCurrentWifiList(const QList<WifiItem *> &value)
{
    m_currentWifiList = value;
}
