import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtMultimedia 5.4
import com.phototwix.components 1.0

import "./resources/controls"

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
                id:isMirror
                Component.onCompleted: {
                    checked = parameters.isMirror;
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
                placeholderText: qsTr("192.168.30.1")
                Component.onCompleted: {
                    text = parameters.commandIP;
                }
            }          
        }

        Row {
            anchors.left: parent.left
            spacing: 10

            Label {
                height: 30
                width: 200
                text: "Mirror IP :"
                font.pixelSize: 15
            }

            TextField {
                id:mirrorIP
                width: 300
                placeholderText: qsTr("192.168.30.2")
                Component.onCompleted: {
                    text = parameters.mirrorIP;
                }
            }
        }

        Row {
            Button {
                text:"Sauvegarder"
                onClicked: {
                    var needSetIP = false;
                    var currentIsMirror = parameters.isMirror;
                    var currentCommendIP = parameters.commandIP;
                    var currentMirrorIP = parameters.mirrorIP;

                    parameters.isMirror = isMirror.checked;
                    parameters.commandIP = commandIP.text;
                    parameters.mirrorIP = mirrorIP.text;

                    if (currentIsMirror != parameters.isMirror) {
                        needSetIP = true;
                    }

                    if (parameters.isMirror && (parameters.mirrorIP != currentMirrorIP)) {
                        needSetIP = true;
                    }

                    if (!parameters.isMirror && (parameters.commandIP != currentCommendIP)) {
                        needSetIP = true;
                    }

                    if (needSetIP) {
                        if (parameters.isMirror) {
                            parameters.wifiManager.changeEthIP(parameters.mirrorIP);
                        } else {
                            parameters.wifiManager.changeEthIP(parameters.commandIP);
                        }
                    }
                }
            }
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
