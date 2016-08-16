import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import QtQuick.Dialogs 1.0

import com.phototwix.components 1.0

import "./resources/controls"

Rectangle {
    id: configTemplateScreen
    color: globalVar.backColor
    height: parent.height
    width: parent.width

    property var currentFrame: null
    property int currentFrameNumber : 0
    property Template currentTemplate

    property alias switchCutter : templateCutterSwitch.checked
    property alias switchDouble : templateDoubleSwitch.checked
    property alias switchLandscape : templateLandscapeSwitch.checked

    function updateTemplatePhotoPositionsRepeater() {
        if (currentTemplate) {
            templatePhotoPositionsRepeater.model = currentTemplate.templatePhotoPositions;
        }
    }

    MouseArea {
        anchors.fill: parent
    }



    Rectangle {
        id: configTemplateScreenButtons
        anchors.left: parent.left
        height: parent.height
        width: parent.width * 0.20

        Column {
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter : parent.horizontalCenter
            spacing: 20

            Button {
                id: addPhoto;
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Ajouter";
                onClicked:
                {
                    currentTemplate.addTemplatePhotoPosition();
                    updateTemplatePhotoPositionsRepeater();
                }
            }

            Button {
                id: deletePhoto;
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Supprimer";
                onClicked:
                {
                    if (currentFrame) {
                        currentTemplate.deleteTemplatePhotoPosition(currentFrameNumber);
                        updateTemplatePhotoPositionsRepeater();
                    }
                }
            }

            Button {
                id: updateTemplate;
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Changer image";
                onClicked:
                {
                    updateTemplateFileDialog.open();
                }
            }

            Grid {
                columns: 2
                columnSpacing: 10
                Label {
                    height: 30
                    text: "Cutter print"
                    font.pixelSize: 15
                }


                Switch {
                    id:templateCutterSwitch
                    onCheckedChanged: {
                        currentTemplate.printcutter = checked
                    }
                    Component.onCompleted: {
                        //checked = currentTemplate.printcutter
                    }
                }

                Label {
                    height: 30
                    text: "Double print"
                    font.pixelSize: 15
                }

                Switch {
                    id:templateDoubleSwitch
                    onCheckedChanged: {
                        currentTemplate.doubleprint = templateDoubleSwitch.checked
                    }
                    Component.onCompleted: {
                        //checked = currentTemplate.doubleprint
                    }
                }

                Label {
                    height: 30
                    text: "Landscape"
                    font.pixelSize: 15
                }


                Switch {
                    id:templateLandscapeSwitch
                    onCheckedChanged: {
                        currentTemplate.landscape = checked
                    }
                    Component.onCompleted: {
                        //checked = currentTemplate.printcutter
                    }
                }

                Label {
                    height: 30
                    text: "Txt color"
                    font.pixelSize: 15
                }

                Rectangle {
                    id:templateTextColor
                    height: 30
                    width: 30
                    border.color: "black"
                    border.width: 2
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            textColorDialog.visible = true
                        }
                    }
                    Component.onCompleted: {
                        if (currentTemplate) {
                            color = currentTemplate.twitterMessageColor
                        }
                    }
                }
            }

            Button {
                id: saveTemplate;
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Sauvegarder";
                onClicked:
                {
                    parameters.Serialize();
                    configGalleryScreen.state = ""
                }
            }

            FileDialog {
                id: updateTemplateFileDialog
                title: "Mise à jour de de template"
                folder: "/media/usb"
                visible:false
                selectMultiple: false
                selectExisting: true
                modality: Qt.NonModal
                nameFilters: [ "Images (*.jpg *.png)" ]
                onAccepted: {
                    currentTemplate.updateImageFromUrl(updateTemplateFileDialog.fileUrl);
                }
            }

            ColorDialog {
                id: textColorDialog
                title: "Choisissez la couleur du texte"
                onAccepted: {
                    templateTextColor.color = textColorDialog.color
                    currentTemplate.twitterMessageColor = textColorDialog.color
                    textColorDialog.visible = false;
                }
                onRejected: {

                }
            }
        }
    }

    Rectangle {
        id: configTemplateScreenTemplate

        property int highestZ: 0

        color: globalVar.backColor
        anchors.right: parent.right
        height: parent.height
        width: parent.width - configTemplateScreenButtons.width

        Image {
            id: currentEditedTemplate

            source: currentTemplate ? currentTemplate.url : ""
            antialiasing: true
            asynchronous: true
            cache: false
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height * 0.80
            width: sourceSize.height > 0 ? sourceSize.width / sourceSize.height * height : 0 //conserv aspect ratio. Can't use native fonction, coords incorect

            onSourceChanged: {
                updateTemplatePhotoPositionsRepeater();
            }

            Repeater { //Affichage de tout les photo frame
                id:templatePhotoPositionsRepeater
                anchors.fill: parent

                model: currentTemplate ? currentTemplate.templatePhotoPositions : ""

                Rectangle {
                    id:templatePhotoPosition
                    y: currentEditedTemplate.height * modelData.y
                    x: currentEditedTemplate.width * modelData.x
                    z:10
                    height: currentEditedTemplate.height * modelData.height
                    width: height * (parameters.cameraWidth / parameters.cameraHeight)
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
