import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtMultimedia 5.4


import "./resources/controls"

Item {
    anchors.topMargin: 15

    MouseArea {
        anchors { fill: parent;  }
        onClicked: {}
    }

    Grid {
       columns: 2
       columnSpacing: 10
       rowSpacing: 10

       Label {
           height: 30
           text: "Flip Camera"
           font.pixelSize: 15
       }

       Switch {
           onCheckedChanged: {
               parameters.flipcamera = checked;
           }
           Component.onCompleted: {
               checked = parameters.flipcamera;
           }
       }

       Label {
           height: 30
           text: "Flip Camera Result"
           font.pixelSize: 15
       }

       Switch {
           onCheckedChanged: {
               parameters.flipresult = checked;
           }
           Component.onCompleted: {
               checked = parameters.flipresult;
           }
       }

       Label {
           height: 30
           text: "Camera Res"
           font.pixelSize: 15
           visible: admin
       }

       Row {
           spacing: 10

           TextField {
               id:editCameraHeight

               Component.onCompleted: {
                   text = parameters.cameraHeight;
               }
           }

           Label {
               text:"x"
           }

           TextField {
               id:editCameraWidth

               Component.onCompleted: {
                   text = parameters.cameraWidth;
               }
           }

           Label {
               text:" (ratio:" + Math.round((parameters.cameraWidth / parameters.cameraHeight) * 100) / 100 + ")"
           }

           Button {
               text:"Save Res"
               onClicked: {
                   parameters.cameraWidth = parseInt(editCameraWidth.text)
                   parameters.cameraHeight = parseInt(editCameraHeight.text)
               }
           }

           Button {
               text:"Focus"
               onClicked: {
                   timerPhotoPreview.start()
                   focusRectangle.visible = true
               }
           }
       }
    }

    Rectangle {
        id: focusRectangle

        anchors.fill: parent
        color: "#D0AAAAAA"
        visible: false

        MouseArea {
            anchors { fill: parent;  }
            onClicked: {}
        }


        Image {
            id: cameraFocusOutput
            source:"image://camerapreview/1"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height * 0.6
            fillMode: Image.PreserveAspectFit
        }

        Timer {
            id: timerPhotoPreview
            interval: 100;
            running: false;
            repeat: true
            onTriggered: {
                cameraWorker.capturePreview();
                cameraFocusOutput.source = "image://camerapreview/" + Math.random();
                // change URL to refresh image. Add random URL part to avoid caching
            }
        }

        ButtonAwesome {
            anchors.right: parent.right
            anchors.top : parent.top
            anchors.rightMargin: 10
            anchors.topMargin: 10
            size: 60
            code:"\uf015"

            onClicked: {
                /*
                timerPhotoPreview.stop()
                cameraWorker.closeCamera();
                focusRectangle.visible = false
                */
            }
        }
/*
        Rectangle {
            anchors.right: parent.right
            anchors.top : parent.top
            anchors.rightMargin: 10
            anchors.topMargin: 10
            height: 60
            width: 60
            color:"#212126"
            radius:height / 7

            Text {

                color: "white"
                height: parent.height
                width: parent.height
                fontSizeMode: Text.Fit
                font.pixelSize: height * 0.8
                font.family: "FontAwesome"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "\uf015"
            }

            MouseArea {
                anchors { fill: parent;  }
                onClicked: {
                    timerPhotoPreview.stop()
                    cameraWorker.closeCamera();
                    focusRectangle.visible = false
                }
            }
        }
*/
    }

}
