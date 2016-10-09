#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <QObject>
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"
#include "wifiitem.h"
#include "parameters.h"

using namespace std;
using namespace rapidjson;

class WifiManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> currentWifiList READ getCurrentWifiList WRITE setCurrentWifiList NOTIFY currentWifiListChanged)
    Q_PROPERTY(QString connectedWifi READ getConnectedWifi WRITE setConnectedWifi NOTIFY connectedWifiChanged)
    Q_PROPERTY(int connectWifiQuality READ getConnectWifiQuality WRITE setConnectWifiQuality NOTIFY connectWifiQualityChanged)
    Q_PROPERTY(QString connectWifiIP READ getConnectWifiIP WRITE setConnectWifiIP NOTIFY connectWifiIPChanged)
    Q_PROPERTY(QString connectEthernetIP READ getConnectedEthernetIP WRITE setConnectedEthernetIP NOTIFY connectEthernetIPChanged)
public:
    explicit WifiManager(QObject *parent = 0);
    WifiManager(Parameters *parameters);

    Q_INVOKABLE void refreshWifiList();
    Q_INVOKABLE void connectWifi(WifiItem *wifi);
    Q_INVOKABLE void changeEthIP(QString IP);

    void serialize(PrettyWriter<StringBuffer>& writer) const;
    void unserialize(Value const &values);

    /**
     * @brief Accessors
     */
    WifiItem *getCurrentWifi() const;
    void setCurrentWifi(WifiItem *value);

    QList<QObject *> getKnownWifi() const;
    void setKnownWifi(const QList<QObject *> &value);

    QList<QObject *> getCurrentWifiList() const;
    void setCurrentWifiList(const QList<QObject *> &value);

    QString getConnectedWifi() const;
    void setConnectedWifi(const QString &connectedWifi);

    int getConnectWifiQuality() const;
    void setConnectWifiQuality(int connectWifiQuality);

    QString getConnectWifiIP() const;
    void setConnectWifiIP(const QString &connectWifiIP);

    QString getConnectedEthernetIP() const;
    void setConnectedEthernetIP(const QString &connectedEthernetIP);

private:
    WifiItem *          m_currentWifi;
    QList<QObject *>    m_knownWifi;
    QList<QObject *>    m_currentWifiList;
    Parameters  *       m_parameters;
    QTimer              m_checkNetworkConnected;

    QString             m_connectedWifi;
    int                 m_connectWifiQuality;
    QString             m_connectWifiIP;
    QString             m_connectedEthernetIP;
    QProcess *          m_connectedWifiCheckProcess;
    QProcess *          m_connectedWifiCheckIP;
    QProcess *          m_connectedEthernetCheckIP;

signals:
    void    currentWifiListChanged();
    void    wifiTryConnect(QString wifiName);
    void    connectedWifiChanged();
    void    connectWifiQualityChanged();
    void    connectWifiIPChanged();
    void    connectEthernetIPChanged();

public slots:
    void    checkWifiConnectedTerminate(int exitCode, QProcess::ExitStatus exitStatus);
    void    checkNetworkConnected();
    void    checkIPWlan0Terminate(int exitCode, QProcess::ExitStatus exitStatus);
    void    checkIPEth0Terminate(int exitCode, QProcess::ExitStatus exitStatus);
};

#endif // WIFIMANAGER_H
