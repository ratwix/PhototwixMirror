import QtQuick 2.0
import QtQuick.Controls 1.4

Item {
    property    int step
    property    bool admin : false
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
                parameters.nbfreephotos = parameters.nbfreephotos - step < 0 ? 0 : parameters.nbfreephotos - step;
            }
        }

        Text {
            anchors.leftMargin: 30
            height: parent.height
            width: parent.height * 3
            font.pixelSize: parent.height * 0.9
            text: parameters.nbfreephotos
        }

        Button {
            height: parent.height
            width: parent.height
            text: "+"
            visible: admin
            onClicked: {
                parameters.nbfreephotos = parameters.nbfreephotos + step
            }
        }
    }
}

