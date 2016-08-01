import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQml.Models 2.2
import "./resources/controls"


Item {
    id:configEffects
    anchors.fill: parent

    Rectangle {
        anchors.fill: parent
        color: "yellow"
    }

    ListModel {
        id:effectButtonModel
        ListElement { name: "Couleur"; }
        ListElement { name: "Sepia"; }
        ListElement { name: "Edge";}
        ListElement { name: "Inkwell"}
        ListElement { name: "1977"}
        ListElement { name: "Amaro"}
        ListElement { name: "Branna"}
        ListElement { name: "Early Bird"}
        ListElement { name: "Hefe"}
        ListElement { name: "Hudson"}
        ListElement { name: "Lomo"}
        ListElement { name: "Lord Kelvin"}
        ListElement { name: "Nashville"}
        ListElement { name: "Pixel"}
        ListElement { name: "Rise"}
        ListElement { name: "Sierra"}
        ListElement { name: "Sutro"}
        ListElement { name: "Toaster"}
        ListElement { name: "Valancia"}
        ListElement { name: "Walden"}
        ListElement { name: "XPro"}
    }

    DelegateModel {
        id:effectButtonDelegate
        model:effectButtonModel
        delegate: ConfigFilterElement {
            height: 200
            width: height * 4
            effectName: name
        }
    }

    ListView {
        orientation: Qt.Vertical
        anchors.fill: parent
        model:effectButtonDelegate
    }
}
