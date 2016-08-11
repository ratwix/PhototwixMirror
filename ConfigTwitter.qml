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
        id: twitterCol
        anchors.left: parent.left
        anchors.top: parent.top
        width: parent.width / 2
        spacing: 5

        Row {
            anchors.left: parent.left
            spacing: 10

            Label {
                id:twitterOnlyLabel
                height: 30
                width: 200
                text: "Borne Twitter seulement :"
                font.pixelSize: 15
            }

            Switch {
                id:twitterTwitterOnly
                Component.onCompleted: {
                    checked = parameters.twitterTwitterOnly;
                }
                onCheckedChanged: {
                    parameters.twitterTwitterOnly = checked;
                }
            }
            /*
        }

        Row {
            anchors.left: parent.left
            spacing: 10
*/
            Label {
                height: 30
                //width: 120
                text: "Ecouter Twitter :  "
                font.pixelSize: 15
            }

            Switch {
                id:twitterListenTwitter
                Component.onCompleted: {
                    checked = parameters.twitterListenTwitter;
                }
                onCheckedChanged: {
                    parameters.twitterListenTwitter = checked;
                }
            }

        }

        Row {
            anchors.left: parent.left
            spacing: 10
            visible: admin

            Label {
                height: 30
                width: 200
                text: "ID Twitter Key :"
                font.pixelSize: 15
            }

            TextField {
                id:twitterKey
                width: 400
                placeholderText: qsTr("Key")
                Component.onCompleted: {
                    text = parameters.twitterKey;
                }
            }
        }

        Row {
            anchors.left: parent.left
            spacing: 10
            visible: admin

            Label {
                height: 30
                width: 200
                text: "ID Twitter Secret :"
                font.pixelSize: 15
            }

            TextField {
                id:twitterSecret
                width: 400
                placeholderText: qsTr("Secret")
                Component.onCompleted: {
                    text = parameters.twitterSecret;
                }
            }
        }

        Row {
            anchors.left: parent.left
            spacing: 10

            Label {
                height: 30
                width: 200
                text: "Twitter Account :"
                font.pixelSize: 15
            }

            TextField {
                id:twitterAccount
                width: 150
                placeholderText: qsTr("@phototwix")
                Component.onCompleted: {
                    text = parameters.twitterAccount;
                }
            }
            /*
        }

        Row {
            anchors.left: parent.left
            spacing: 10
*/
            Label {
                height: 30
                //width: 100
                text: "Twitter Tag :  "
                font.pixelSize: 15
            }

            TextField {
                id:twitterTag
                width: 150
                placeholderText: qsTr("#party")
                Component.onCompleted: {
                    text = parameters.twitterTag;
                }
            }
        }

        Row {
            anchors.left: parent.left
            spacing: 10

            Label {
                height: 30
                width: 200
                text: "Twitter Message :"
                font.pixelSize: 15
            }

            TextField {
                id:twitterMessage
                width: 400
                placeholderText: qsTr("%1 %2 Mon message")
                Component.onCompleted: {
                    text = parameters.twitterMessage;
                }
            }
        }

        Row {
            anchors.left: parent.left
            spacing: 10

            Label {
                height: 30
                width: 200
                text: "Twitter Message Color :"
                font.pixelSize: 15
            }

            Rectangle {
                id:twitterMessageColor
                height: 30
                width: 30
                border.color: "black"
                border.width: 2
                Component.onCompleted: {
                    color = parameters.twitterMessageColor
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        textColorTwitterDialog.open();
                    }
                }
            }
        }

        Button {
            text:"Sauvegarder parametres Twitter"
            onClicked: {
                parameters.twitterTwitterOnly = twitterTwitterOnly.checked;
                parameters.twitterListenTwitter = twitterListenTwitter.checked;
                parameters.twitterKey = twitterKey.text;
                parameters.twitterSecret = twitterSecret.text;
                parameters.twitterAccount = twitterAccount.text;
                parameters.twitterTag = twitterTag.text;
                parameters.twitterMessage = twitterMessage.text;
                parameters.twitterMessageColor = twitterMessageColor.color;
                parameters.Serialize();
            }
        }
    }

    ColorDialog {
        id: textColorTwitterDialog
        title: "Choisissez la couleur du texte twitter"
        onAccepted: {
            twitterMessageColor.color = textColorTwitterDialog.color
            textColorTwitterDialog.visible = false
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
