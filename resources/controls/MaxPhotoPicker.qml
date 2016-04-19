import QtQuick 2.0
import QtQuick.Controls 1.4

Item {
    property    int     step
    property    bool    block
    property    bool    admin:false

    Row {
        spacing: 5
        height: parent.height
        Switch {
            checked:block
            visible: admin
            onCheckedChanged: {
                parameters.blockPrint = checked;
            }
            Component.onCompleted: {
                checked = parameters.blockPrint;
            }
        }

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
                    parameters.blockPrintNb = parameters.blockPrintNb - step < 0 ? 0 : parameters.blockPrintNb - step;
                }
            }
            Text {
                anchors.leftMargin: 30
                height: parent.height
                width: parent.height * 3
                font.pixelSize: parent.height * 0.9
                text: parameters.blockPrintNb
            }
            Button {
                height: parent.height
                width: parent.height
                text: "+"
                visible: admin
                onClicked: {
                    parameters.blockPrintNb = parameters.blockPrintNb + step
                }
            }
        }
    }
}
