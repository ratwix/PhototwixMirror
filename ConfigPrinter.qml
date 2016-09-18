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

    Grid {
       columns: 2
       columnSpacing: 10
       rowSpacing: 10

       Label {
           height: 30
           text: "Nombre d'impressions"
           font.pixelSize: 15
       }

       Rectangle {
           height: 30
           width: 150
           color:"transparent"
           Text {
               anchors.left: parent.left
               id:tnbprint
               height: 30
               font.pixelSize: 30 * 0.9
               text: parameters.nbprint
           }

           Button {
               anchors.left: tnbprint.right
               anchors.leftMargin: 20
               height: 30
               text: "Remise à 0"
               visible: admin
               onClicked: {
                   parameters.nbprint = 0;
               }
           }
       }

       Label {
           height: 30
           text: "Papier Restant"
           font.pixelSize: 15
       }

       Rectangle {
           height: 30
           width: 150
           color:"transparent"
           Text {
               anchors.left: parent.left
               id:tpaperprint
               height: 30
               font.pixelSize: 30 * 0.9
               text: parameters.printerManager.paperprint
           }

           Button {
               anchors.left: tpaperprint.right
               anchors.leftMargin: 20
               height: 30
               text: "Remise à jour"
               onClicked: {
                   parameters.printerManager.updatePaperPrint();
               }
           }
       }

       Label {
           height: 30
           text: "Prix supplémentaire"
           font.pixelSize: 15
       }

       Text {
           height: 30
           font.pixelSize: 30 * 0.9
           text: parameters.nbprint - parameters.nbfreephotos > 0 ? ((parameters.nbprint - parameters.nbfreephotos) * parameters.pricephoto).toFixed(2).toString() + "€" : "0€";
       }


       Label {
           height: 30
           text: "Photos gratuites"
           font.pixelSize: 15
       }
       //FreePhotoNumberPicker

       FreePhotoPicker {
           id:freePhotoNumber
           height: 30
           width: 150
           step:10
           admin: configScreen.admin
       }

       Label {
           height: 30
           text: "Prix par photo supplementaires"
           font.pixelSize: 15
       }

       PricePhotoPicker {
           id:pricePhoto
           height: 30
           width: 150
           step:0.05
           admin: configScreen.admin
       }

       Label {
           height: 30
           text: "Bloquer impression"
           font.pixelSize: 15
       }

       MaxPhotoPicker {
           id:maxPhotoNumber
           block:false
           height: 30
           width: 300
           step:10
           admin:configScreen.admin
       }

       Label {
           height: 30
           text: "Auto Print"
           font.pixelSize: 15
       }

       Switch {
           id:autoPrintSwitch
           onCheckedChanged: {
               parameters.autoPrint = checked
           }
           Component.onCompleted: {
               checked = parameters.autoPrint
           }
       }

       Label {
           height: 30
           text: "Auto Print Delay (s)"
           font.pixelSize: 15
       }

       Row {
           id:autoPrintDelay
           height: 30
           spacing: 0

           Button {
               height: parent.height
               width: parent.height
               text: "-"
               onClicked: {
                   parameters.autoPrintDelay = parameters.autoPrintDelay > 0 ? parameters.autoPrintDelay - 1 : 0
               }
           }
           Text {
               anchors.leftMargin: 30
               height: parent.height
               width: parent.height * 3
               font.pixelSize: parent.height * 0.9
               text: parameters.autoPrintDelay
           }
           Button {
               height: parent.height
               width: parent.height
               text: "+"
               onClicked: {
                   parameters.autoPrintDelay = parameters.autoPrintDelay + 1
               }
           }
       }
    }
}
