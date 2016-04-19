import QtQuick 2.4
import QtQuick.Controls 1.4
import QtMultimedia 5.4

Item {
    id:configVideo
    state: "NORMAL"
    anchors.topMargin: 15

    property alias videoPreview: previewPlayer

    MouseArea {
        anchors { fill: parent;  }
        onClicked: {}
    }

    Column {
        id: videoConfigColumn
        spacing: 10


        ConfigVideoItem {
            id:waitVideo
            height: 50
            videoLabel: "Wait Video:"
            videoItem: parameters.waitVideo
        }

        ConfigVideoItem {
            id:startGlobalPhotoProcessVideo
            height: 50
            videoLabel: "Start Global Photo Process:"
            videoItem: parameters.startGlobalPhotoProcessVideo
        }

        ConfigVideoItem {
            id:startPhotoProcessVideo
            height: 50
            videoLabel: "Start Photo Process:"
            videoItem: parameters.startPhotoProcessVideo
        }

        ConfigVideoItem {
            id:endGlobalPhotoProcessVideo
            height: 50
            videoLabel: "End Global Photo Process:"
            videoItem: parameters.endGlobalPhotoProcessVideo
        }
    }

    Rectangle {
        id:backPreview
        visible: false
        color: "white"
        anchors.fill: parent
    }

    MediaPlayer {
        id: previewPlayer
        loops: MediaPlayer.Infinite
        autoPlay: false
    }

    VideoOutput {
        visible: false
        id: previewVideoOutput
        source: previewPlayer
        anchors.fill: parent
    }

    Rectangle {
        id:previewButtonClose
        visible: false
        height: 50
        width: 50
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 10
        anchors.rightMargin: 10
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
            text: "\uf00d"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                configVideo.state = "NORMAL";
                previewPlayer.stop();
            }
        }
    }

    states: [
            State {
                name: "NORMAL"
            },
            State {
                name: "PREVIEW"
                PropertyChanges { target: previewButtonClose; visible: true}
                PropertyChanges { target: previewVideoOutput; visible: true}
                PropertyChanges { target: backPreview; visible: true}
            }
        ]
}
