import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQml.Models 2.2
import QtQuick.Particles 2.0

import "../controls"

Item {
    id:activeTemplateItem

    property int last_template_index: -1
    //property var currentActiveTemplates : parameters.activesTemplates


    Component {
        id: activeTemplateDelegate

        Item {
            id: activeTemplateDelegateItem
            height: parent.height * 0.9
            width: templateSelect.paintedWidth  //parent.height * 0.9
            scale: PathView.iconScale
            opacity: PathView.iconOpacity
            z: PathView.z
            property real rotationAngle: PathView.angle
            transform: Rotation {
                    origin.x: templateSelect.width / 2
                    origin.y: templateSelect.height / 2
                    axis { x: 0; y: 1; z: 0 } // Rotate around y-axis.
                    angle: rotationAngle
            }

            //Template last selection color
            Rectangle {
                color:activeTemplateDelegateItem.PathView.isCurrentItem ? globalVar.backColorTemplateLast : globalVar.backColorTemplate
                anchors.fill: templateSelect
            }

            //Template image
            Image {
                id: templateSelect
                source: modelData.url
                height: parent.height
                fillMode: Image.PreserveAspectFit
                cache: true
                asynchronous: false
                antialiasing: true
            }

            Text {
                id:templateSelectTwitterDefaultIcon
                color: "white"
                height: parent.height * 0.1
                width: templateSelectTwitterDefaultIcon.height
                anchors.top : templateSelect.top
                anchors.right: parent.right
                fontSizeMode: Text.Fit
                font.pixelSize: height
                font.family: "FontAwesome"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "\uf081"
                visible: modelData.twitterDefault
            }
        }
    }



    PathView {
            anchors.fill: parent
            model: parameters.activesTemplates
            delegate: activeTemplateDelegate
            path: coverFlowPath
            preferredHighlightBegin: 0.5
            preferredHighlightEnd: 0.5
            focus: true
            onMovementEnded: {
                last_template_index = currentIndex
            }
    }

    Path {
            id: coverFlowPath
            // "Start zone"
            startX: -25
            startY: activeTemplateItem.height / 2
            PathAttribute { name: "z"; value: 0 }
            PathAttribute { name: "angle"; value: 70 }
            PathAttribute { name: "iconScale"; value: 0.6 }
            PathAttribute { name: "iconOpacity"; value: 0.4 }
            // Just before middle
            PathLine { x: activeTemplateItem.width * 0.35; y: activeTemplateItem.height / 2;  }
            PathAttribute { name: "z"; value: 50 }
            PathAttribute { name: "angle"; value: 45 }
            PathAttribute { name: "iconScale"; value: 0.85 }
            PathAttribute { name: "iconOpacity"; value: 0.6 }
            PathPercent { value: 0.40 }
            // Middle
            PathLine { x: activeTemplateItem.width * 0.5; y: activeTemplateItem.height / 2;  }
            PathAttribute { name: "z"; value: 100 }
            PathAttribute { name: "angle"; value: 0 }
            PathAttribute { name: "iconScale"; value: 1.0 }
            PathAttribute { name: "iconOpacity"; value: 1.0 }
            // Just after middle
            PathLine { x: activeTemplateItem.width * 0.65; y: activeTemplateItem.height / 2; }
            PathAttribute { name: "z"; value: 50 }
            PathAttribute { name: "angle"; value: -45 }
            PathAttribute { name: "iconScale"; value: 0.85 }
            PathAttribute { name: "iconOpacity"; value: 0.6 }
            PathPercent { value: 0.60 }
            // Final stop
            PathLine { x: activeTemplateItem.width + 25; y: activeTemplateItem.height / 2; }
            PathAttribute { name: "z"; value: 0 }
            PathAttribute { name: "angle"; value: -70 }
            PathAttribute { name: "iconScale"; value: 0.6 }
            PathAttribute { name: "iconOpacity"; value: 0.4 }
            PathPercent { value: 1.0 }
        }
}

