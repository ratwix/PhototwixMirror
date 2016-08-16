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
    property alias timerStartPrint: timerStartTimerPrint
    property alias timerStartClose: timerStartTimerClose

    Item {
        id:templatePreviewItem
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.width * 0.90

        /*
        TemplatePreview {
            id:resultPhoto
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height * 0.8
            photo: currentPhoto
            visible: currentPhoto ? (currentPhoto.finalResult == "" ? true : false) : false
        }
        */

        TemplatePreview {
            id:resultPhoto
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            height: 1300
            photo: currentPhoto
            visible: currentPhoto ? (currentPhoto.finalResult == "" ? true : false) : false
        }

        ShaderEffectSource {
            id:resultPhotoShader
            height: parent.height * 0.8
            width: resultPhoto.height > 0 ? resultPhoto.width / resultPhoto.height * resultPhotoShader.height : 0
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            visible: currentPhoto ? (currentPhoto.finalResult == "" ? true : false) : false
            sourceItem: resultPhoto
            hideSource: true
        }

        Image {
            id:resultPhotoImage
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height * 0.8
            fillMode: Image.PreserveAspectFit
            visible: currentPhoto ? (currentPhoto.finalResult == "" ? false : true) : false
            cache: false
            asynchronous: true
            source: currentPhoto ? currentPhoto.finalResult : ""
        }

        BusyIndicator {
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height * 0.5
            width: parent.height * 0.5
            running: resultPhotoShader.visible || resultPhotoImage.status === Image.Loading
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
                visible: resultPhotoImage.visible
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
                visible: resultPhotoImage.visible
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
                visible: resultPhotoImage.visible
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
            parameters.printerManager.updatePaperPrint();
            if (parameters.printerManager.paperprint < 15 && parameters.printerManager.paperprint > 2) { //Warning if paper become low
                mbox.message = "Impression en cours.\nPlus que " + parameters.printerManager.paperprint + " feuilles"
                mbox.imageTag = "\uf02f"
                mbox.state = "show"
                printPhoto()
            } else {
                if (parameters.printerManager.paperprint < 2) {
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
        parameters.printerManager.printPhoto(currentPhoto.finalResult,
                              currentPhoto.currentTemplate.doubleprint,
                              currentPhoto.currentTemplate.printcutter,
                              currentPhoto.currentTemplate.landscape)
    }

    //Timer qui verifie toute les 0.5s si la photo est chargée
    Timer {
        id:timerStartTimerPrint
        interval: 500
        running: false
        repeat: true

        onTriggered: {
            //Si la photo est visible, demarage du timer
            if (currentPhoto && (currentPhoto.finalResult != "") && (resultPhotoImage.status !== Image.Loading)) {
                //Arret du timer actuel
                timerStartTimerPrint.stop();
                //Demarage auto print
                if (parameters.autoPrint) {
                    timerAutoPrint.start()
                }
            }
        }
    }

    Timer {
        id:timerStartTimerClose
        interval: 500
        running: false
        repeat: true

        onTriggered: {
            //Si la photo est visible, demarage du timer
            if (currentPhoto && (currentPhoto.finalResult != "") && (resultPhotoImage.status !== Image.Loading)) {
                //Arret du timer actuel
                timerStartTimerClose.stop();
                //Demarage auto close
                if (parameters.showPhotoDelay > 0) {
                    timerClosePhoto.start()
                }
            }
        }
    }

    Timer {
        id:timerClosePhoto
        interval: parameters.showPhotoDelay * 1000
        running: false
        repeat: false
        triggeredOnStart: false

        onTriggered: {
            timerAutoPrint.stop();
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
            if (parameters.autoPrint) {
                testPrintPhoto();
            }
        }
    }

    //Timeline to auto disepear
}
