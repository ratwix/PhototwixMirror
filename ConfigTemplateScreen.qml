
import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2

import "../resources/controls"

Rectangle {
    id: configTemplateScreen

    height: parent.height
    width: parent.width

    property var currentFrame: null
    property int currentFrameNumber : 0

    function updateTemplatePhotoPositionsRepeater() {
        templatePhotoPositionsRepeater.model = applicationWindows.currentEditedTemplate.templatePhotoPositions;
    }

    MouseArea {
        anchors.fill: parent
    }

    Rectangle {
        id: configTemplateScreenButtons
        anchors.left: parent.left
        height: parent.height
        width: parent.width * 0.15

        Column {
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter : parent.horizontalCenter
            spacing: 20

            ButtonImage {
                id: addPhoto;
                anchors.horizontalCenter: parent.horizontalCenter
                label: "Ajouter";
                onClicked:
                {
                    applicationWindows.currentEditedTemplate.addTemplatePhotoPosition();
                    updateTemplatePhotoPositionsRepeater();
                }
            }

            ButtonImage {
                id: deletePhoto;
                anchors.horizontalCenter: parent.horizontalCenter
                label: "Supprimer";
                onClicked:
                {
                    if (currentFrame) {
                        applicationWindows.currentEditedTemplate.deleteTemplatePhotoPosition(currentFrameNumber);
                        updateTemplatePhotoPositionsRepeater();
                    }
                }
            }

            ButtonImage {
                id: saveTemplate;
                anchors.horizontalCenter: parent.horizontalCenter
                label: "Sauvegarder";
                onClicked:
                {
                    parameters.Serialize();
                    mainRectangle.state = "CONFIG"
                }
            }

            ButtonImage {
                id: updateTemplate;
                anchors.horizontalCenter: parent.horizontalCenter
                label: "Changer image";
                onClicked:
                {
                    updateTemplateFileDialog.open();
                }
            }

            FileDialog {
                id: updateTemplateFileDialog
                title: "Mise à jour de de template"
                folder: shortcuts.home
                visible:false
                selectMultiple: false
                selectExisting: true
                modality: Qt.NonModal
                nameFilters: [ "Images (*.jpg *.png)" ]
                onAccepted: {
                    applicationWindows.currentEditedTemplate.updateImageFromUrl(updateTemplateFileDialog.fileUrl);
                }
            }
        }
    }

    Rectangle {
        id: configTemplateScreenTemplate

        property int highestZ: 0


        color: applicationWindows.backColor
        anchors.right: parent.right
        height: parent.height
        width: parent.width - configTemplateScreenButtons.width

        Image {
            id: currentEditedTemplate

            source: applicationWindows.currentEditedTemplate ? applicationWindows.currentEditedTemplate.url : ""
            antialiasing: true
            asynchronous: true
            cache: false
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height * 0.95
            width: sourceSize.width / sourceSize.height * height //conserv aspect ratio. Can't use native fonction, coords incorect

            onSourceChanged: {
                updateTemplatePhotoPositionsRepeater();
            }

            Repeater { //Affichage de tout les photo frame
                id:templatePhotoPositionsRepeater
                anchors.fill: parent

                model: applicationWindows.currentEditedTemplate ? applicationWindows.currentEditedTemplate.templatePhotoPositions : ""

                Rectangle {
                    id:templatePhotoPosition
                    y: currentEditedTemplate.height * modelData.y
                    x: currentEditedTemplate.width * modelData.x
                    z:10
                    height: currentEditedTemplate.height * modelData.height
                    width: height * applicationWindows.cameraRation
                    rotation: modelData.rotate
                    color: "#800000FF"
                    smooth: true
                    antialiasing: true
                    property bool renderFinish: false
                    property alias sliderCameraCutterAlias: sliderCameraCutter
                    onXChanged: {
                        if (renderFinish) {
                            calculateCoord()
                        }
                    }

                    onYChanged: {
                        if (renderFinish) {
                            calculateCoord()
                        }
                    }

                    onRotationChanged: {
                        if (renderFinish) {
                            modelData.rotate = rotation;
                        }
                    }

                    onHeightChanged: {
                        if (renderFinish) {
                            modelData.height = templatePhotoPosition.height / currentEditedTemplate.height
                        }
                    }

                    onWidthChanged: {
                        if (renderFinish) {
                            modelData.width = templatePhotoPosition.width / currentEditedTemplate.width
                        }
                    }

                    Component.onCompleted: {
                        renderFinish = true
                    }

                    function calculateCoord() {
                        var tx = templatePhotoPosition.x;
                        var ty = templatePhotoPosition.y;

                        var maxx = currentEditedTemplate.width
                        var maxy = currentEditedTemplate.height

                        var px = tx / maxx;
                        var py = ty / maxy;

                        modelData.x = px;
                        modelData.y = py;
                    }

                    Label {

                        id: templatePhotoPositionNumber
                        anchors.centerIn: parent
                        font.pixelSize: parent.height * 0.6
                        text: modelData.number
                    }


                    PinchArea {
                        anchors.fill: parent
                        pinch.target: templatePhotoPosition
                        pinch.minimumRotation: -180
                        pinch.maximumRotation: 180
                        pinch.minimumScale: 1
                        pinch.maximumScale: 1
                        onPinchStarted: setFrameColor();

                        MouseArea {
                            id: dragArea
                            hoverEnabled: true
                            anchors.fill: parent
                            drag.target: templatePhotoPosition
                            onPressed: {
                                templatePhotoPosition.z = ++configTemplateScreenTemplate.highestZ; //set element to top
                                parent.setFrameColor();
                            }
                            onEntered: {
                                currentFrameNumber = modelData.number;
                                parent.setFrameColor();
                            }

                            onWheel: {
                                if (wheel.modifiers & Qt.ControlModifier) {
                                    templatePhotoPosition.rotation += wheel.angleDelta.y / 120;
                                    if (Math.abs(templatePhotoPosition.rotation) < 0.6)
                                        templatePhotoPosition.rotation = 0;
                                } else {
                                    templatePhotoPosition.rotation += wheel.angleDelta.x / 120;
                                    if (Math.abs(templatePhotoPosition.rotation) < 0.6)
                                        templatePhotoPosition.rotation = 0;
                                    var heighBefore = templatePhotoPosition.height;
                                    var widthBefore = templatePhotoPosition.width;
                                    templatePhotoPosition.height += templatePhotoPosition.height * wheel.angleDelta.y / 120 / 60;
                                    templatePhotoPosition.width =  widthBefore / heighBefore * templatePhotoPosition.height
                                    templatePhotoPosition.x -= (templatePhotoPosition.width - widthBefore) / 2.0;
                                    templatePhotoPosition.y -= (templatePhotoPosition.height - heighBefore) / 2.0;
                                }
                            }
                        }

                        function setFrameColor() {
                            if (currentFrame) {
                                currentFrame.color = "#800000FF";
                                currentFrame.sliderCameraCutterAlias.visible = false;
                            }
                            currentFrame = templatePhotoPosition;
                            currentFrame.color = "#80FF0000";
                            sliderCameraCutter.visible = true;
                        }

                    }

                    //Slider to cut camera
                    Slider {
                        id:sliderCameraCutter
                        anchors.top: parent.bottom
                        anchors.left: parent.left
                        width: parent.width / 2
                        minimumValue: 0.0
                        maximumValue: 0.5
                        visible: false
                        Component.onCompleted: {
                            value = modelData.xphoto;
                        }
                        onValueChanged: {
                            cutLine.requestPaint();
                            modelData.xphoto = value;
                        }
                    }

                    Canvas {
                        id:cutLine
                        anchors.fill: parent
                        height: parent.height
                        width:parent.width
                        onPaint: {
                            var context = getContext("2d");
                            //Clear
                            context.fillStyle = Qt.rgba(255, 255, 255, 1.0);
                            context.strokeStyle = "green";
                            context.lineWidth = 3;

                            context.beginPath();
                            context.clearRect(0, 0, cutLine.width, cutLine.height);
                            context.fill();


                            //Draw first line
                            context.beginPath();
                            context.moveTo(cutLine.width * sliderCameraCutter.value, 0);
                            context.lineTo(cutLine.width * sliderCameraCutter.value, cutLine.height);
                            context.stroke();

                            //Draw second line
                            context.beginPath();
                            context.moveTo(cutLine.width - cutLine.width * sliderCameraCutter.value, 0);
                            context.lineTo(cutLine.width - cutLine.width * sliderCameraCutter.value, cutLine.height);
                            context.stroke();
                        }
                    }
                }
            }
        }
    }
}
