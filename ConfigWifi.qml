import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtMultimedia 5.4

import com.phototwix.components 1.0

import "./resources/controls"

Item {

    MouseArea {
        anchors.fill: parent
        onClicked: {

        }
    }

    Component {
        id: wifiListDelegate
        Item {
            width: 500; height: 40
            Row {
                height: parent.height
                spacing: 10

                Text {
                    color: (model.modelData.wifiQuality > 75) ? "green" : ((model.modelData.wifiQuality > 45) ? "orange" : "red")
                    height: parent.height
                    width: height
                    font.pixelSize: height * 0.8
                    font.family: "FontAwesome"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: "\uf1eb "
                }

                Column {
                    width: 200
                    Text { text: model.modelData.wifiESSID }
                    Text { text: model.modelData.wifiAddress }
                }

                TextField {
                    id:wifiPassword
                    placeholderText: "Password"
                    width: 200
                    text: model.modelData.wifiPassword
                }
                ButtonAwesome {
                    id:connectButton
                    height: parent.height
                    width:height
                    code:"\uf1e6"
                    onClicked: {
                        model.modelData.wifiPassword = wifiPassword.text
                        parameters.wifiManager.connectWifi(model.modelData)
                    }
                }
            }
        }
    }

    ListView {
        id: wifiListItem
        anchors.top: parent.top
        anchors.bottom: keyboard.top
        anchors.bottomMargin: keyboard.height / 4
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 10
        model:parameters.wifiManager.currentWifiList
        delegate: wifiListDelegate
    }

    //Connexion wifi actuelle

    Column {
        anchors.bottom: keyboard.top
        anchors.right: parent.right
        anchors.bottomMargin: 10
        anchors.rightMargin: 10
        width: parent.height * 0.3

        Text {
            width: parent.width
            height: width
            font.pixelSize: height
            font.family: "FontAwesome"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: "\uf1eb "
            color:(parameters.wifiManager.connectWifiQuality > 75) ? "green" :  ((parameters.wifiManager.connectWifiQuality > 45) ? "orange" : (parameters.wifiManager.connectWifiQuality > 0) ? "red" : "black")
        }
        Text {
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text:parameters.wifiManager.connectedWifi
        }
        Text {
            width: parent.width
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            text:"WIFI:" + parameters.wifiManager.connectWifiIP
        }
        Text {
            width: parent.width
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            text:"Eth:" + parameters.wifiManager.connectEthernetIP
        }
    }

    ButtonAwesome {
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        height: 60
        width: 60
        code: "\uf021"
        onClicked: {
            parameters.wifiManager.refreshWifiList();
        }
    }

    InputPanel {
        id:keyboard
        state:"SHOW"

        onEsc: {

        }

        onEnter: {

        }
    }
}
