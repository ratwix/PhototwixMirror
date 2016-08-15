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
public:
    explicit WifiManager(QObject *parent = 0);
    WifiManager(Parameters *parameters);

    Q_INVOKABLE void refreshWifiList();
    Q_INVOKABLE void connectWifi(WifiItem *wifi);

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

private:
    WifiItem *          m_currentWifi;
    QList<QObject *>    m_knownWifi;
    QList<QObject *>    m_currentWifiList;
    Parameters  *       m_parameters;
    QTimer              m_checkWifiConnected;

    QString             m_connectedWifi;
    int                 m_connectWifiQuality;
    QProcess *          m_connectedWifiCheckProcess;

signals:
    void    currentWifiListChanged();
    void    wifiTryConnect(QString wifiName);
    void    connectedWifiChanged();
    void    connectWifiQualityChanged();
public slots:
    void    checkWifiConnectedTerminate(int exitCode, QProcess::ExitStatus exitStatus);
    void    checkWifiConnected();
};

#endif // WIFIMANAGER_H
