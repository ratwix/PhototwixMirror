import QtQuick 2.5
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import com.phototwix.components 1.0

import "./resources/controls"

Item {
    //Template view
    property Photo currentPhoto
    property alias timerShow : timerClosePhoto
    property alias timerPrint: timerAutoPrint

    Item {
        id:templatePreviewItem
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.width * 0.90

        TemplatePreview {
            id:resultPhoto
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height * 0.8
            //width: parent.width * 0.8
            photo: currentPhoto
            visible: currentPhoto ? (currentPhoto.finalResult == "" ? true : false) : false
        }

        Image {
            id:resultPhotoImage
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height * 0.8
            fillMode: Image.PreserveAspectFit
            visible: !resultPhoto.visible
            source: currentPhoto ? currentPhoto.finalResult : ""
        }
    }

    //Button home / Print / Delete / Mail

    Item {
        id:actionButtonItem
        anchors.left: templatePreviewItem.right
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
                    timerClosePhoto.stop();
                    timerAutoPrint.stop();
                    commandScreenItem.state = "CHOOSE_TEMPLATE"
                }
            }

            ButtonAwesome {
                id:printResultButton
                size: actionButton.width
                code:"\uf02f"

                onClicked: {
                    timerAutoPrint.stop();
                    if (parameters.showPhotoDelay > 0) {
                        timerClosePhoto.restart();
                    }
                    testPrintPhoto();
                }
            }

            ButtonAwesome {
                id:deleteResultButton
                size: actionButton.width
                code:"\uf1f8"

                onClicked: {
                    timerAutoPrint.stop();
                    if (parameters.showPhotoDelay > 0) {
                        timerClosePhoto.restart();
                    }
                    if (currentPhoto) {
                        cbox.message = "Supprimer la photo ?"
                        cbox.acceptFunction = deletePhoto
                        cbox.state = "show"
                    }
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

    function deletePhoto() {
        timerClosePhoto.stop();
        commandScreenItem.state = "CHOOSE_TEMPLATE"
        parameters.photoGallery.removePhoto(currentPhoto.name);
    }

    function testPrintPhoto() {
        if (parameters.blockPrint && parameters.nbprint >= parameters.blockPrintNb) {
            mbox.message = "Plus d'impressions disponibles..."
            mbox.imageTag = "\uf02f"
            mbox.state = "show"
        } else {
            parameters.updatePaperPrint();
            if (parameters.paperprint < 15) { //Warning if paper become low
                mbox.message = "Impression en cours.\nPlus que " + parameters.paperprint + " feuilles"
                mbox.imageTag = "\uf02f"
                mbox.state = "show"
                printPhoto()
            } else {
                if (parameters.paperprint < 2) {
                    mbox.message = "Plus de papier"
                    mbox.imageTag = "\uf02f"
                    mbox.state = "show"
                } else {
                    mbox.message = "Impression en cours"
                    mbox.imageTag = "\uf02f"
                    mbox.state = "show"
                    printPhoto()
                }
            }
        }
    }

    function printPhoto() {
        parameters.printPhoto(currentPhoto.finalResult,
                              currentPhoto.currentTemplate.doubleprint,
                              currentPhoto.currentTemplate.printcutter,
                              currentPhoto.currentTemplate.landscape)
    }

    Timer {
        id:timerClosePhoto
        interval: parameters.showPhotoDelay * 1000
        running: false
        repeat: false
        triggeredOnStart: false

        onTriggered: {
            timerClosePhoto.stop();
            commandScreenItem.state = "CHOOSE_TEMPLATE"
        }
    }

    //Auto print
    Timer {
        id:timerAutoPrint
        interval: parameters.autoPrintDelay * 1000
        running: false
        repeat: false
        triggeredOnStart: false

        onTriggered: {
            timerAutoPrint.stop();
            if (parameters.autoPrint) {
                testPrintPhoto();
            }
        }
    }

    //Timeline to auto disepear
}
