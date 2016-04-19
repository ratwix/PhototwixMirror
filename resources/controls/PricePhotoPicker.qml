import QtQuick 2.0
import QtQuick.Controls 1.4

Item {
    property    real step
    property bool admin: false
    Grid {
        columns: 3
        columnSpacing: 0
        height: parent.height
        Button {
            height: parent.height
            width: parent.height
            text: "-"
            visible: admin
            onClicked: {
                parameters.pricephoto = parameters.pricephoto - step < 0 ? 0 : parameters.pricephoto - step;
            }
        }
        Text {
            anchors.leftMargin: 30
            height: parent.height
            width: parent.height * 3
            font.pixelSize: parent.height * 0.9
            text: parameters.pricephoto.toFixed(2) + "€"
        }
        Button {
            height: parent.height
            width: parent.height
            visible: admin
            text: "+"
            onClicked: {
                parameters.pricephoto = parameters.pricephoto + step
            }
        }
    }
}

