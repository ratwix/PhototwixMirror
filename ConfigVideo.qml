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
            id:twitterVideo
            height: 50
            videoLabel: "Twitter Video:"
            videoItem: parameters.twitterVideo
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
            height: 30
            Label {
                width: 200
                text: "Loop Wait -> Twitter"
            }

            Button {
                height: parent.height
                width: parent.height
                text: "-"
                onClicked: {
                    parameters.waitVideoRepeatBeforeTwitter = parameters.waitVideoRepeatBeforeTwitter - 1 < 1 ? 1 : parameters.waitVideoRepeatBeforeTwitter - 1;
                }
            }

            Text {
                anchors.leftMargin: 30
                height: parent.height
                width: parent.height * 3
                font.pixelSize: parent.height * 0.9
                text: parameters.waitVideoRepeatBeforeTwitter
            }

            Button {
                height: parent.height
                width: parent.height
                text: "+"
                onClicked: {
                    parameters.waitVideoRepeatBeforeTwitter = parameters.waitVideoRepeatBeforeTwitter + 1
                }
            }

        }

        Row {
            height: 30
            Label {
                width: 200
                text: "Photo Countdown (s)"
            }

            Button {
                height: parent.height
                width: parent.height
                text: "-"
                onClicked: {
                    parameters.countdown = parameters.countdown - 1 < 0 ? 0 : parameters.countdown - 1;
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
            height: 30
            Label {
                width: 200
                text: "View Photo (s)"
            }

            Button {
                height: parent.height
                width: parent.height
                text: "-"
                onClicked: {
                    parameters.viewPhotoTime = parameters.viewPhotoTime - 1 < 0 ? 0 : parameters.viewPhotoTime - 1;
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
            height: 30
            Label {
                width: 200
                text: "View All Photos (s)"
            }

            Button {
                height: parent.height
                width: parent.height
                text: "-"
                onClicked: {
                    parameters.viewAllPhotoTime = parameters.viewAllPhotoTime - 1 < 0 ? 0 : parameters.viewAllPhotoTime - 1;
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
