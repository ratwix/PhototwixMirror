import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQml.Models 2.2
import QtQuick.Particles 2.0

import "../controls"

Item {
    property int last_template_index: -1
    //property var currentActiveTemplates : parameters.activesTemplates


    Component {
        id: activeTemplateDelegate

        //Last template have particle system. This is this one that will be selected when press button
        Item {
            height: parent.height * 0.9
            width: templateSelect.width

            //Template last selection color
            Rectangle {
                color:(index == last_template_index) ? globalVar.backColorTemplateLast : globalVar.backColorTemplate
                anchors.fill: parent
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

                MouseArea {
                    id: templateSelectMouseArea
                    anchors.fill: parent

                    onClicked: {
                        /* TODO : On Click template
                        if ((mainRectangle.state == "START")) {
                            mainRectangle.state = "TAKE_PHOTO"
                            applicationWindows.currentPhoto = parameters.addPhotoToGallerie("Test", model.modelData)
                            cameraWorker.capturePreview()
                            applicationWindows.effectSource = "Couleur"
                            last_template_index = index
                        }
                        */
                        last_template_index = index
                    }
                }
            }
        }
    }


    Row {
        height: parent.height
        anchors.horizontalCenter:parent.horizontalCenter
        spacing: 20
        Repeater {
            model: parameters.activesTemplates
            delegate: activeTemplateDelegate
        }
    }
}

