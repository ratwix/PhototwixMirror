import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtMultimedia 5.4

import com.phototwix.components 1.0


import "./resources/controls"

Item {
    id: configGalleryScreen

    anchors.topMargin: 15
    state: ""

    MouseArea {
        anchors { fill: parent;  }
        onClicked: {}
    }

    Column {
        id:controlConfigGallery
        spacing: 5

        Row {
            anchors.left: parent.left
            spacing: 10

            Label {
                height: 30
                text: "Gallerie:"
                font.pixelSize: 15
            }

            Button {
                text:"Nettoyer"
                visible: admin
                onClicked: {
                    console.debug("Reset photos");
                    cbox.message = "Vider la gallerie ?"
                    cbox.acceptFunction = function () {
                        parameters.clearGallery();
                        mbox.message = "La gallerie a été effacée"
                        mbox.state = "show"
                    }
                    cbox.state = "show"
                }
            }

            Button {
                text:"Nettoyer et effacer"
                visible: admin
                onClicked: {
                    console.debug("Reset photos & delete");
                    cbox.message = "Vider la gallerie et supprimer les photos et mails ?"
                    cbox.acceptFunction = function () {
                        parameters.clearGalleryDeleteImages();
                        mbox.message = "La gallerie les photos et mes mailsont étés effacés"
                        mbox.state = "show"
                    }
                    cbox.state = "show"
                }
            }
        }

        Row {
            anchors.left: parent.left
            spacing: 10


            Column {
                anchors.leftMargin: 10
                spacing: 5

                CheckBox {
                    id: copySingle
                    text: "Sans visuel"
                    checked: true
                }

                CheckBox {
                    id: copyDeleted
                    text: "Supprime"
                    checked: true
                }

                CheckBox {
                    id: copyDeletedSingle
                    text: "Supprime sans visuel"
                    checked: true
                }
            }

            Button {
                id:saveGalleryButton
                text:"Sauvegarder"
                onClicked: {
                    console.debug("Save Gallery");
                    saveGallerie.open()
                }
            }



            /*
                function getReadableFileSizeString(fileSizeInBytes) {

                    var i = -1;
                    var byteUnits = [' kB', ' MB', ' GB', ' TB', 'PB', 'EB', 'ZB', 'YB'];
                    do {
                        fileSizeInBytes = fileSizeInBytes / 1024;
                        i++;
                    } while (fileSizeInBytes > 1024);

                    return Math.max(fileSizeInBytes, 0.1).toFixed(1) + byteUnits[i];
                };
            */
        }

        Row {
            anchors.left: parent.left
            spacing: 10

            Button {
                text:"Importer des visuels"
                onClicked: {
                    importTemplateFileDialog.open()
                }
            }

            Button {
                id:buttonBackground
                text:"Fond d'écran"
                onClicked: {
                    importBackgroundFileDialog.open()
                }
            }
        }

        Row {
            anchors.left: parent.left
            spacing: 10

            Label {
                height: 30
                text: "Delais Voir Photo (s)"
                font.pixelSize: 15
            }

            Row {
                id:showPhotoDelay
                height: 30
                spacing: 0

                Button {
                    height: parent.height
                    width: parent.height
                    text: "-"
                    onClicked: {
                        parameters.showPhotoDelay = parameters.showPhotoDelay > 5 ? parameters.showPhotoDelay - 5 : 0
                    }
                }
                Text {
                    anchors.leftMargin: 30
                    height: parent.height
                    width: parent.height * 3
                    font.pixelSize: parent.height * 0.9
                    text: parameters.showPhotoDelay
                }
                Button {
                    height: parent.height
                    width: parent.height
                    text: "+"
                    visible: admin
                    onClicked: {
                        parameters.showPhotoDelay = parameters.showPhotoDelay + 5
                    }
                }
            }
        }
    }

    /**
      * Template view
      */

    ListView {
        id:templateConfigGallery
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.rightMargin: parent.width * 0.1
        height: parent.height
        width: parent.width * 0.4
        spacing: 30


        ExclusiveGroup { id: templateDefaultSwitchGroup }
        //Dans quel répertoire il faut chercher

        //Représentation des template, avec un bouton et un switch
        Component {
            id: fileDelegate

            Row {
                spacing : 20
                anchors.leftMargin: 20
                Column {
                    width: 150
                    spacing: 10
                    anchors.verticalCenter: parent.verticalCenter

                    BusyIndicator {
                        anchors.horizontalCenter: parent.horizontalCenter
                        running: templateImage.status === Image.Loading
                    }

                    Image {
                        id:templateImage
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: model.modelData.url
                        height: 100
                        fillMode: Image.PreserveAspectFit
                        cache: false
                        asynchronous: true
                        antialiasing: true
                    }

                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr(model.modelData.name)
                    }
                }

                Column {
                    anchors.verticalCenter: parent.verticalCenter
                    spacing : 10

                    Grid {
                        columns: 2
                        columnSpacing: 10
                        Label {
                            height: 30
                            text: "Active"
                            font.pixelSize: 15
                        }

                        Switch {
                            id:templateActiveSwitch
                            onCheckedChanged: {
                                model.modelData.active = checked
                            }
                            Component.onCompleted: {
                                checked = model.modelData.active
                            }
                        }

                        Label {
                            height: 30
                            text: "Twitter Default"
                            font.pixelSize: 15
                        }

                        RadioButton {
                            id:templateDefaultSwitch
                            exclusiveGroup: templateDefaultSwitchGroup

                            onCheckedChanged: {
                                model.modelData.twitterDefault = checked
                            }
                            Component.onCompleted: {
                                checked = model.modelData.twitterDefault
                            }
                        }
                    }

                    Button {
                        text: "Config"

                        property Template templateItem: model.modelData
                        onClicked: {
                            configTemplate.currentTemplate = templateItem
                            configTemplate.switchCutter = templateItem.printcutter
                            configTemplate.switchDouble = templateItem.doubleprint
                            configTemplate.switchLandscape = templateItem.landscape
                            configGalleryScreen.state = "CONFIG_TEMPLATE"
                        }
                    }

                    Button {
                        visible: admin
                        text: "Supprimer"
                        onClicked: {
                            console.debug("Delete template");
                            cbox.message = "Supprimer ce visuel ?"
                            cbox.acceptFunction = function () {
                                parameters.deleteTemplateFromName(model.modelData.name);
                            }
                            cbox.state = "show"
                        }
                    }
                }
            }
        }

        model: parameters.templates
        delegate: fileDelegate
    }

    ConfigTemplateScreen {
        id:configTemplate
        anchors.fill: parent
        visible:false
    }


    FileDialog {
        id: importTemplateFileDialog
        title: "Import de template"
        folder: shortcuts.home
        visible:false
        selectMultiple: true
        selectExisting: true
        modality: Qt.NonModal
        nameFilters: [ "Images (*.jpg *.png)" ]
        onAccepted: {
            console.debug("Add templates: " + importTemplateFileDialog.fileUrls);
            for (var i = 0; i < importTemplateFileDialog.fileUrls.length; i++) {
                parameters.addTemplateFromUrl(importTemplateFileDialog.fileUrls[i]);
            }
        }
    }

    FileDialog {
        id: importBackgroundFileDialog
        title: "Import Fond d'écran"
        folder: shortcuts.home
        visible:false
        selectMultiple: false
        selectExisting: true
        modality: Qt.NonModal
        nameFilters: [ "Images (*.jpg *.png)" ]
        onAccepted: {
            parameters.changeBackground(importBackgroundFileDialog.fileUrl);
        }
    }

    FileDialog {
        id: saveGallerie
        title: "Sauvegarde"
        folder: shortcuts.home
        visible:false
        selectMultiple: false
        selectFolder: true
        selectExisting: true
        modality: Qt.NonModal
        onAccepted: {
            parameters.photoGallery.saveGallery(saveGallerie.fileUrl, copySingle.checked, copyDeleted.checked, copyDeletedSingle.checked);
        }
    }

    Connections {
        target: photoGallery
        onCopyStart: {
            mbox.message = "Copy en cours"
            mbox.state = "showNoTimer"
        }
    }

    Connections {
        target: photoGallery
        onCopyEnd: {
            mbox.state = "hide"
        }
    }

    Connections {
        target: photoGallery
        onCopyProgress: {
            mbox.message = "Copy " + photoGallery.currentCopy + " / " + photoGallery.totalFileNumber;
        }
    }

    states: [
        State {
            name: "CONFIG_TEMPLATE"
            PropertyChanges {
                target: configTemplate
                visible: true
            }
        }
    ]
}
