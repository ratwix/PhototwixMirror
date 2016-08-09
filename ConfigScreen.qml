import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtMultimedia 5.4


import "./resources/controls"

Rectangle {
    id: configScreen
    color: globalVar.backColor
    height: parent.height
    width: parent.width

    property bool admin: false

    signal currentEditedTemplateChange(url currentUrl)

    MouseArea {
        anchors.fill: parent
    }

    state: "FULL"

    TabView {
        id: configTabView
        anchors.left: parent.left
        anchors.top: parent.top
        height: parent.height
        width:parent.width * 0.9

        Tab {
            title: "Impression"
            ConfigPrinter {
                id:configPrinter
                anchors.fill: parent
            }
        }

        Tab {
            title: "Camera"
            ConfigCamera {
                id:configCamera
                anchors.fill: parent
            }
        }

        Tab {
            title: "Gallerie & Visuels"
            ConfigGallery {
                id:configGallery
                anchors.fill: parent
            }
        }

        Tab {
            title: "Twitter"
            ConfigTwitter {
                id:configTwitter
                anchors.fill: parent
            }
        }

        Tab {
            title: "Mail"
            ConfigMail {
                id:configMail
                anchors.fill: parent
            }
        }

        Tab {
            title: "Videos"
            ConfigVideo {
                id:configVideo
                anchors.fill: parent
            }
        }

        Tab {
            title: "Effets"
            ConfigEffects {
                id:configEffets
                anchors.fill: parent
            }
        }
    }

    Column {
        anchors.left: configTabView.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 15
        height: parent.height
        spacing:15

        ButtonAwesome {
            id:homeButton
            anchors.horizontalCenter: parent.horizontalCenter
            size: parent.width * 0.85
            code: "\uf015"
            onClicked: {
                commandScreenItem.state = "CHOOSE_TEMPLATE"
            }
        }

        ButtonAwesome {
            id:quitButton
            anchors.horizontalCenter: parent.horizontalCenter
            size: parent.width * 0.85
            code: "\uf08b"
            onClicked: {
                Qt.quit()
            }
        }

        ButtonAwesome {
            id:haltButton
            anchors.horizontalCenter: parent.horizontalCenter
            size: parent.width * 0.85
            code: "\uf011"
            onClicked: {
                parameters.haltSystem()
                Qt.quit()
            }
        }

    }

    states: [
        State {
            name: "FULL"
        },

        State {
            name: "FOCUS"
            PropertyChanges { target: focusRectangle; visible:true}
        }
    ]


}
