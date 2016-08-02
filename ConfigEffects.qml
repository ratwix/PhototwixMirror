import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQml.Models 2.2
import "./resources/controls"


Item {
    id:configEffects
    anchors.fill: parent

    MouseArea {
        anchors { fill: parent;  }
        onClicked: {}
    }

    DelegateModel {
        id:effectButtonDelegate
        //model:effectButtonModel
        model:parameters.effects
        delegate: ConfigFilterElement {
            height: 200
            width: height * 4
            effectName: model.modelData.effectName
        }
    }

    ListView {
        orientation: Qt.Vertical
        anchors.fill: parent
        model:effectButtonDelegate

        ExclusiveGroup {id: effectTwitterDefaultGroup}
    }
}
