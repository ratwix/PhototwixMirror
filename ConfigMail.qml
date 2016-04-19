import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtMultimedia 5.4

import com.phototwix.components 1.0

import "./resources/controls"

Item {
    id:mailRectangle
    anchors.topMargin: 15

    MouseArea {
        anchors { fill: parent;  }
        onClicked: {}
    }

    Column {
        id: mailCol
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        width: parent.width / 2
        spacing: 5

        Button {
            text:"Sauvegarder Mails"
            visible: admin
            onClicked: {
                saveMail.open()
            }
        }
        Row {
            anchors.left: parent.left
            spacing: 10

            Label {
                id:mailLabel
                height: 30
                text: "Mail :"
                font.pixelSize: 15
            }

            Switch {
                id:mailActive

                Component.onCompleted: {
                    checked = parameters.mailActive;
                }
            }
        }

        Row {
            anchors.left: parent.left
            spacing: 10
            visible: admin

            Label {
                id:fromMailLabel
                height: 30
                text: "Mail From :"
                font.pixelSize: 15
            }

            TextField {
                id:mailFrom

                width: 300
                placeholderText: qsTr("From")
                Component.onCompleted: {
                    text = parameters.mailFrom;
                }
            }
        }

        Row {
            anchors.left: parent.left
            spacing: 10

            Label {
                id:ccMailLabel
                height: 30
                text: "Mail Cc :"
                font.pixelSize: 15
            }

            TextField {
                id:mailCc
                width: 300
                placeholderText: qsTr("Cc")
                Component.onCompleted: {
                    text = parameters.mailCc;
                }
            }
        }

        Row {
            anchors.left: parent.left
            spacing: 10

            Label {
                id:bccMailLabel
                height: 30
                text: "Mail Bcc :"
                font.pixelSize: 15
            }

            TextField {
                id:mailBcc
                width: 300
                placeholderText: qsTr("Bcc")
                Component.onCompleted: {
                    text = parameters.mailBcc;
                }
            }
        }

        Row {
            anchors.left: parent.left
            spacing: 10
            visible: admin

            Label {
                height: 30
                text: "SMTP :"
                font.pixelSize: 15
            }

            TextField {
                id:mailSmtp
                visible: admin
                width: 300
                placeholderText: qsTr("Smtp")
                Component.onCompleted: {
                    text = parameters.mailSmtp;
                }
            }

            TextField {
                id:mailPort
                visible: admin
                width: 50
                placeholderText: qsTr("Port")
                Component.onCompleted: {
                    text = parameters.mailPort;
                }
            }

            TextField {
                id:mailUsername
                visible: admin
                width: 150
                placeholderText: qsTr("User")
                Component.onCompleted: {
                    text = parameters.mailUsername;
                }
            }

            TextField {
                id:mailPassword
                visible: admin
                width: 150
                placeholderText: qsTr("Password")
                Component.onCompleted: {
                    text = parameters.mailPassword;
                }
            }
        }

        Row {
            anchors.left: parent.left
            spacing: 10

            Label {
                id:mailSubjectLabel
                height: 30
                text: "Sujet :"
                font.pixelSize: 15
            }

            TextField {
                id:mailSubject
                width: 300
                placeholderText: qsTr("Subject")
                Component.onCompleted: {
                    text = parameters.mailSubject;
                }
            }
        }

        Row {
            anchors.left: parent.left
            spacing: 10

            Label {
                id:mailContentLabel
                height: 30
                text: "Contenu :"
                font.pixelSize: 15
            }

            TextArea {
                id:mailContent
                width: 300
                height: 200
                Component.onCompleted: {
                    text = parameters.mailContent;
                }
            }
        }

        Row {
            anchors.left: parent.left
            spacing: 10

            Button {
                text:"Sauvegarder"

                onClicked: {
                    parameters.mailFrom = mailFrom.text
                    parameters.mailCc = mailCc.text
                    parameters.mailBcc = mailBcc.text
                    parameters.mailSmtp = mailSmtp.text
                    parameters.mailPort = mailPort.text
                    parameters.mailUsername = mailUsername.text
                    parameters.mailPassword = mailPassword.text
                    parameters.mailSubject = mailSubject.text
                    parameters.mailContent = mailContent.text
                    parameters.mailActive = mailActive.checked
                    parameters.Serialize();
                    configScreen.state = "FULL"
                }
            }

            Button {
                text:"Annuler"
                onClicked: {
                    mailFrom.text = parameters.mailFrom
                    mailCc.text = parameters.mailCc
                    mailBcc.text = parameters.mailBcc
                    mailSmtp.text = parameters.mailSmtp
                    mailPort.text = parameters.mailPort
                    mailUsername.text = parameters.mailUsername
                    mailPassword.text = parameters.mailPassword
                    mailSubject.text = parameters.mailSubject
                    mailContent.text = parameters.mailContent
                    mailActive.checked = parameters.mailActive
                    configScreen.state = "FULL"
                }
            }
        }
    }

    InputPanel {
        id:keyboard
        state:"SHOW"

        onEsc: {
            configScreen.state = "FULL"
        }

        onEnter: {

        }
    }

    FileDialog {
        id: saveMail
        title: "Sauvegarde mails"
        folder: shortcuts.home
        visible:false
        selectMultiple: false
        selectFolder: true
        selectExisting: true
        modality: Qt.NonModal
        onAccepted: {
            mail.saveMail(saveMail.fileUrl);
        }
    }

}
