import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtMultimedia 5.4


import "./resources/controls"

Item {
    anchors.topMargin: 15

    MouseArea {
        anchors { fill: parent;  }
        onClicked: {}
    }

    Column {
        id: mailCol
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
                        mail.resetMail();
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

            Button {
                text:"Sauvegarder"
                onClicked: {
                    console.debug("Save Gallery");
                    saveGallerie.open()
                }
            }

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
}
