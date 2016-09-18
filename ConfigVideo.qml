import QtQuick 2.4
import QtQuick.Controls 1.4
import QtMultimedia 5.0

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
            videoLabel: "Start Photo:"
            videoItem: parameters.startGlobalPhotoProcessVideo
        }

        ConfigVideoItem {
            id:endGlobalPhotoProcessVideo
            height: 50
            videoLabel: "End Video:"
            videoItem: parameters.endGlobalPhotoProcessVideo
        }

        Row {
            height: 50
            Label {
                width: 200
                text: "Photo Countdown"
            }

            Button {
                height: parent.height
                width: parent.height
                text: "-"
                onClicked: {
                    parameters.countdown = parameters.countdown - 1 < 0 ? 0 : parameters.countdown;
                }
            }

            Text {
                anchors.leftMargin: 30
                height: parent.height
                width: parent.height * 3
                font.pixelSize: parent.height * 0.9
                text: parameters.countdown
            }

            Button {
                height: parent.height
                width: parent.height
                text: "+"
                onClicked: {
                    parameters.countdown = parameters.countdown + 1
                }
            }

        }

        Row {
            height: 50
            Label {
                width: 200
                text: "View Photo"
            }

            Button {
                height: parent.height
                width: parent.height
                text: "-"
                onClicked: {
                    parameters.viewPhotoTime = parameters.viewPhotoTime - 1 < 0 ? 0 : parameters.viewPhotoTime;
                }
            }

            Text {
                anchors.leftMargin: 30
                height: parent.height
                width: parent.height * 3
                font.pixelSize: parent.height * 0.9
                text: parameters.viewPhotoTime
            }

            Button {
                height: parent.height
                width: parent.height
                text: "+"
                onClicked: {
                    parameters.viewPhotoTime = parameters.viewPhotoTime + 1
                }
            }

        }

        Row {
            height: 50
            Label {
                width: 200
                text: "View All Photos"
            }

            Button {
                height: parent.height
                width: parent.height
                text: "-"
                onClicked: {
                    parameters.viewAllPhotoTime = parameters.viewAllPhotoTime - 1 < 0 ? 0 : parameters.viewAllPhotoTime;
                }
            }

            Text {
                anchors.leftMargin: 30
                height: parent.height
                width: parent.height * 3
                font.pixelSize: parent.height * 0.9
                text: parameters.viewAllPhotoTime
            }

            Button {
                height: parent.height
                width: parent.height
                text: "+"
                onClicked: {
                    parameters.viewAllPhotoTime = parameters.viewAllPhotoTime + 1
                }
            }
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
