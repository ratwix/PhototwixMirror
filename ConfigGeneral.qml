import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtMultimedia 5.4

Item {
    anchors.fill: parent
    anchors.topMargin: 15

    MouseArea {
        anchors.fill: parent
    }

    Column {
        anchors.left: parent.left
        anchors.top: parent.top
        width: parent.width
        spacing: 5

        Row {
            anchors.left: parent.left
            spacing: 10

            Label {
                height: 30
                width: 200
                text: "Mirror :"
                font.pixelSize: 15
            }

            Switch {
                Component.onCompleted: {
                    checked = parameters.isMirror;
                }
                onCheckedChanged: {
                    parameters.isMirror = checked;
                }
            }
        }

        Row {
            anchors.left: parent.left
            spacing: 10

            Label {
                height: 30
                width: 200
                text: "Command IP :"
                font.pixelSize: 15
            }

            TextField {
                id:commandIP
                width: 300
                placeholderText: qsTr("127.0.0.1")
                Component.onCompleted: {
                    text = parameters.commandIP;
                }
            }

            Button {
                text:"Sauvegarder"
                onClicked: {
                    parameters.commandIP = commandIP.text;
                }
            }

        }
    }
}
