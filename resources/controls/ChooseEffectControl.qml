import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQml.Models 2.2

Item {
    id:activeEffectItem

    MouseArea {
        anchors { fill: parent;  }
        onClicked: {}
    }

    Component {
        id: activeEffectDelegate


        FilterElement {
            id: activeEffectDelegateElement
            height: parent.height * 0.6
            width: height * 1.5
            scale: PathView.iconScale
            opacity: PathView.iconOpacity
            z: PathView.z
            property real rotationAngle: PathView.angle
            transform: Rotation {
                    origin.x: activeEffectDelegateElement.width / 2
                    origin.y: activeEffectDelegateElement.height / 2
                    axis { x: 0; y: 1; z: 0 } // Rotate around y-axis.
                    angle: rotationAngle
            }
            effectName: model.modelData.effectName
            effectTwitterDefault : model.modelData.effectTwitterDefault
        }

    }

    PathView {
            anchors.fill: parent
            model: parameters.activeEffects
            delegate: activeEffectDelegate
            path: coverFlowEffectPath
            preferredHighlightBegin: 0.5
            preferredHighlightEnd: 0.5
            pathItemCount : 5
            onMovementEnded: {
                globalVar.currentEffect = parameters.activeEffects[currentIndex].effectName
            }
            Component.onCompleted: {
                globalVar.currentEffect = parameters.activeEffects[currentIndex].effectName
            }
    }

    Path {
            id: coverFlowEffectPath
            // "Start zone"
            startX: -25
            startY: activeEffectItem.height / 2
            PathAttribute { name: "z"; value: 0 }
            PathAttribute { name: "angle"; value: 70 }
            PathAttribute { name: "iconScale"; value: 0.6 }
            PathAttribute { name: "iconOpacity"; value: 0.4 }
            // Just before middle
            PathLine { x: activeEffectItem.width * 0.35; y: activeEffectItem.height / 2;  }
            PathAttribute { name: "z"; value: 50 }
            PathAttribute { name: "angle"; value: 45 }
            PathAttribute { name: "iconScale"; value: 0.85 }
            PathAttribute { name: "iconOpacity"; value: 0.6 }
            PathPercent { value: 0.40 }
            // Middle
            PathLine { x: activeEffectItem.width * 0.5; y: activeEffectItem.height / 2;  }
            PathAttribute { name: "z"; value: 100 }
            PathAttribute { name: "angle"; value: 0 }
            PathAttribute { name: "iconScale"; value: 1.0 }
            PathAttribute { name: "iconOpacity"; value: 1.0 }
            // Just after middle
            PathLine { x: activeEffectItem.width * 0.65; y: activeEffectItem.height / 2; }
            PathAttribute { name: "z"; value: 50 }
            PathAttribute { name: "angle"; value: -45 }
            PathAttribute { name: "iconScale"; value: 0.85 }
            PathAttribute { name: "iconOpacity"; value: 0.6 }
            PathPercent { value: 0.60 }
            // Final stop
            PathLine { x: activeEffectItem.width + 25; y: activeEffectItem.height / 2; }
            PathAttribute { name: "z"; value: 0 }
            PathAttribute { name: "angle"; value: -70 }
            PathAttribute { name: "iconScale"; value: 0.6 }
            PathAttribute { name: "iconOpacity"; value: 0.4 }
            PathPercent { value: 1.0 }
        }
}
