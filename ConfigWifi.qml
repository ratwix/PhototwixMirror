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
            width: 300; height: 40
            Row {
                height: parent.height
                spacing: 10

                Text {
                    color: (model.modelData.wifiQuality > 75) ? "green" : ((model.modelData.wifiQuality > 50) ? "orange" : "red")
                    height: parent.height
                    width: height
                    font.pixelSize: height * 0.8
                    font.family: "FontAwesome"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: "\uf1eb "
                }

                Column {
                    width: 100
                    Text { text: model.modelData.wifiESSID }
                    Text { text: model.modelData.wifiAddress }
                }

                TextField {
                    id:wifiPassword
                    placeholderText: "Password"
                    width: 100
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
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 10
        model:parameters.wifiManager.currentWifiList
        delegate: wifiListDelegate
    }

    ButtonAwesome {
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        height: 30
        width: 30
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