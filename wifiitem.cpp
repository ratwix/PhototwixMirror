#include "wifiitem.h"

WifiItem::WifiItem(QObject *parent) : QObject(parent)
{
    m_wifiHasPassword = true;
}

QString WifiItem::wifiESSID() const
{
    return m_wifiESSID;
}

void WifiItem::setWifiESSID(const QString &wifiESSID)
{
    m_wifiESSID = wifiESSID;
    emit wifiESSIDChanged();
}

QString WifiItem::wifiAddress() const
{
    return m_wifiAddress;
}

void WifiItem::setWifiAddress(const QString &wifiAddress)
{
    m_wifiAddress = wifiAddress;
    emit wifiAddressChanged();
}

QString WifiItem::wifiPassword() const
{
    return m_wifiPassword;
}

void WifiItem::setWifiPassword(const QString &wifiPassword)
{
    m_wifiPassword = wifiPassword;
    emit wifiPasswordChanged();
}

int WifiItem::wifiQuality() const
{
    return m_wifiQuality;
}

void WifiItem::setWifiQuality(int wifiQuality)
{
    m_wifiQuality = wifiQuality;
    emit wifiQualityChanged();
}

bool WifiItem::wifiHasPassword() const
{
    return m_wifiHasPassword;
}

void WifiItem::setWifiHasPassword(bool wifiHasPassword)
{
    m_wifiHasPassword = wifiHasPassword;
    emit wifiPasswordChanged();
}
