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
    Q_PROPERTY(QList<WifiItem *> currentWifiList READ getCurrentWifiList WRITE setCurrentWifiList NOTIFY currentWifiListChanged)
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

    QList<WifiItem *> getKnownWifi() const;
    void setKnownWifi(const QList<WifiItem *> &value);

    QList<WifiItem *> getCurrentWifiList() const;
    void setCurrentWifiList(const QList<WifiItem *> &value);

private:
    WifiItem *          m_currentWifi;
    QList<WifiItem *>   m_knownWifi;
    QList<WifiItem *>   m_currentWifiList;
    Parameters  *       m_parameters;
signals:
    void currentWifiListChanged();
public slots:
};

#endif // WIFIMANAGER_H
