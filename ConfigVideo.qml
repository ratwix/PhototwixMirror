import QtQuick 2.4
import QtQuick.Controls 1.4
import QtMultimedia 5.4

Item {
    id:configVideo
    state: "NORMAL"
    anchors.topMargin: 15
    anchors.fill: parent

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

    MediaPlayer {
        id: previewPlayer
        loops: MediaPlayer.Infinite
        autoPlay: false
    }

    VideoOutput {
        visible: false
        id: previewVideoOutput
        source: previewPlayer
        width: parent.width / 2
        height: parent.height / 2
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        MouseArea {
            anchors.fill: parent;
            onClicked: {
                previewPlayer.stop();
                configVideo.state = "NORMAL"
            }

        }
    }

    states: [
            State {
                name: "NORMAL"
            },
            State {
                name: "PREVIEW"
                PropertyChanges { target: previewVideoOutput; visible: true}
            }
        ]
}
