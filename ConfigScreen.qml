import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtMultimedia 5.4


import "./resources/controls"

Rectangle {
    id: configScreen
    //color: applicationWindows.backColor
    height: parent.height
    width: parent.width

    property bool admin: false

    signal currentEditedTemplateChange(url currentUrl)

    MouseArea {
        anchors.fill: parent
    }

    FontLoader {
        source: "/resources/font/FontAwesome.otf"
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
            title: "Gallery & Visuels"
            ConfigGallery {
                id:configGallery
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
            title: "Mail"
            ConfigMail {
                id:configMail
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

        Rectangle {
            id:homeButton
            anchors.horizontalCenter: parent.horizontalCenter
            height: width
            width: parent.width * 0.85
            color:"#212126"
            radius:height / 7

            Text {
                color: "white"
                height: parent.height
                width: parent.height
                fontSizeMode: Text.Fit
                font.pixelSize: height * 0.8
                font.family: "FontAwesome"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "\uf015" //Home
            }


            MouseArea {
                anchors { fill: parent;  }
                onClicked: { mainRectangle.state = "MIRROR" }
            }
        }

        Rectangle {
            id:quitButton
            anchors.horizontalCenter: parent.horizontalCenter
            height: width
            width: parent.width * 0.85
            color:"#212126"
            radius:height / 7

            Text {
                color: "white"
                height: parent.height
                width: parent.height
                fontSizeMode: Text.Fit
                font.pixelSize: height * 0.8
                font.family: "FontAwesome"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "\uf08b" //Exit
            }


            MouseArea {
                anchors { fill: parent;  }
                onClicked: {
                    Qt.quit()
                }
            }
        }

        Rectangle {
            id:haltButton
            anchors.horizontalCenter: parent.horizontalCenter
            height: width
            width: parent.width * 0.85
            color:"#212126"
            radius:height / 7

            Text {
                color: "white"
                height: parent.height
                width: parent.height
                fontSizeMode: Text.Fit
                font.pixelSize: height * 0.8
                font.family: "FontAwesome"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "\uf011" //Power Off
            }


            MouseArea {
                anchors { fill: parent;  }
                onClicked: {
                    parameters.haltSystem()
                    Qt.quit()
                }
            }
        }
    }

    /*
    //Actions
    Column {
        anchors.left: parent.left
        width: parent.width * 0.4
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 40
        spacing: 15

        Row {
            anchors.left: parent.left
            spacing: 10

            Button {
                //anchors.left: parent.left
                text:"Quitter"
                onClicked: {
                    Qt.quit()
                }
            }

            Button {
                //anchors.left: parent.left
                text:"Eteindre"
                onClicked: {
                    parameters.haltSystem()
                    Qt.quit()
                }
            }
        }
    }


    //Liste des templates




    Rectangle {
        id:homeButton
        anchors.right: parent.right
        anchors.top : parent.top
        anchors.rightMargin: 10
        anchors.topMargin: 10
        height: 60
        width: 60
        color:"#212126"
        radius:height / 7

        Text {
            color: "white"
            height: parent.height
            width: parent.height
            fontSizeMode: Text.Fit
            font.pixelSize: height * 0.8
            font.family: "FontAwesome"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: "\uf015" //Home
        }


        MouseArea {
            anchors { fill: parent;  }
            onClicked: { mainRectangle.state = "MIRROR" }
        }
    }
    */
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
