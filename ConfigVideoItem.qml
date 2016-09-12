import QtQuick 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import com.phototwix.components 1.0

import "./resources/controls"

Rectangle {
    id:configVideoItem
    property alias videoLabel:label.text;
    property VideoItem videoItem;

    anchors.topMargin: 15
    color: "transparent"
    width: 500

    MouseArea {
        anchors.fill: parent
        onClicked: {

        }
    }

    Row {
        anchors.left: parent.left
        spacing: 5

        Label {
            id:label
            height: configVideoItem.height
            width: 150
            text: videoLabel
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id:waitVideoText
            height: configVideoItem.height
            width: 300
            text:videoItem.videoName
            verticalAlignment: Text.AlignVCenter
        }

        ButtonAwesome {
            size: configVideoItem.height
            code: "\uf1c8"
            onClicked: {
                fileDialog.visible = true;
            }
        }


/*
        Rectangle {
            height: configVideoItem.height
            width: configVideoItem.height
            color: "black"
            radius: 10

            Text {
                id: txt
                color: "white"
                height: parent.height
                width: parent.height
                fontSizeMode: Text.Fit
                font.pixelSize: height * 0.8
                font.family: "FontAwesome"

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "\uf1c8"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    fileDialog.visible = true;
                }
            }
        }
*/
        ButtonAwesome {
            size: configVideoItem.height
            code: "\uf04b"
            onClicked: {
                configVideo.videoPreview.source = "file:///" + videoItem.videoPath;
                configVideo.videoPreview.play();
                configVideo.state = "PREVIEW"
            }
        }
/*
        Rectangle {
            height: configVideoItem.height
            width: configVideoItem.height
            color: "black"
            radius: 10

            Text {
                color: "white"
                height: parent.height
                width: parent.height
                fontSizeMode: Text.Fit
                font.pixelSize: height * 0.8
                font.family: "FontAwesome"

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "\uf04b"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.debug("Video source : file:///" + videoItem.videoPath)
                    configVideo.videoPreview.source = "file:///" + videoItem.videoPath;
                    configVideo.videoPreview.play();
                    configVideo.state = "PREVIEW"
                }
            }
        }
*/
    }

    FileDialog {
        id: fileDialog
        title: "Choisir un fichier pour " + videoLabel
        folder: "/media/usb"
        onAccepted: {
            videoItem.videoName = fileDialog.fileUrl
            console.log("You chose: " + fileDialog.fileUrl)

        }
    }
}
