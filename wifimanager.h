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

private:
    WifiItem *          m_currentWifi;
    QList<QObject *>   m_knownWifi;
    QList<QObject *>   m_currentWifiList;
    Parameters  *       m_parameters;

    void                createWifiMokup();

signals:
    void currentWifiListChanged();
public slots:
};

#endif // WIFIMANAGER_H
