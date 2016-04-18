import QtQuick 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import com.phototwix.components 1.0

Rectangle {
    id:configVideoItem
    property alias videoLabel:label.text;

    property VideoItem videoItem;

    color: "transparent"
    width: 500
    FontLoader {
        source: "/resources/font/FontAwesome.otf"
    }

    Row {
        anchors.left: parent.left
        spacing: 5

        Label {
            id:label
            height: configVideoItem.height
            width: 50
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
                    //console.log("Play video : " + videoUrl);
                    configVideo.state = "PREVIEW"
                }
            }
        }

    }

    FileDialog {
        id: fileDialog
        title: "Choisir un fichier pour " + videoLabel
        folder: shortcuts.home
        onAccepted: {
            videoItem.videoName = fileDialog.fileUrl
            console.log("You chose: " + fileDialog.fileUrl)

        }
    }
}
