#ifndef WIFIITEM_H
#define WIFIITEM_H

#include <QObject>

class WifiItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString wifiESSID READ wifiESSID WRITE setWifiESSID NOTIFY wifiESSIDChanged)
    Q_PROPERTY(QString wifiAddress READ wifiAddress WRITE setWifiAddress NOTIFY wifiAddressChanged)
    Q_PROPERTY(QString wifiPassword READ wifiPassword WRITE setWifiPassword NOTIFY wifiPasswordChanged)
    Q_PROPERTY(int wifiQuality READ wifiQuality WRITE setWifiQuality NOTIFY wifiQualityChanged)
    Q_PROPERTY(bool wifiHasPassword READ wifiHasPassword WRITE setWifiHasPassword NOTIFY wifiHasPasswordChanged)
public:
    explicit WifiItem(QObject *parent = 0);

    QString wifiESSID() const;
    void setWifiESSID(const QString &wifiESSID);

    QString wifiAddress() const;
    void setWifiAddress(const QString &wifiAddress);

    QString wifiPassword() const;
    void setWifiPassword(const QString &wifiPassword);

    int wifiQuality() const;
    void setWifiQuality(int wifiQuality);

    bool wifiHasPassword() const;
    void setWifiHasPassword(bool wifiHasPassword);

private:
    QString m_wifiESSID;
    QString m_wifiAddress;
    QString m_wifiPassword;
    int     m_wifiQuality;
    bool    m_wifiHasPassword;

signals:
    void wifiESSIDChanged();
    void wifiAddressChanged();
    void wifiPasswordChanged();
    void wifiQualityChanged();
    void wifiHasPasswordChanged();
public slots:
};

#endif // WIFIITEM_H
