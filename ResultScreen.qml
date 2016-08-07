import QtQuick 2.5
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import com.phototwix.components 1.0

import "./resources/controls"

Item {
    //Template view

    Item {
        id:templatePreview

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.width * 0.80

        Rectangle {
            anchors.fill: parent
            color: "blue"
        }
    }

    Item {
        anchors.left: templatePreview.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom


        Column {
            id:actionButton
            width:parent.width * 0.9
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            //columns: parameters.mailActive ? 3 : 2

            spacing: 30

            ButtonAwesome {
                id:homeesultButton
                size: actionButton.width
                code:"\uf015"

                onClicked: {
                    commandScreenItem.state = "CHOOSE_TEMPLATE"
                }
            }

            ButtonAwesome {
                id:printResultButton
                size: actionButton.width
                code:"\uf02f"

                onClicked: {

                }
            }

            ButtonAwesome {
                id:deleteResultButton
                size: actionButton.width
                code:"\uf1f8"

                onClicked: {

                }
            }

            ButtonAwesome {
                id:mailResultButton
                size: actionButton.width
                visible: parameters.mailActive
                code:"\uf0e0"

                onClicked: {

                }
            }
        }
    }

    //Button print and delete
    //Timeline to auto disepear
}
