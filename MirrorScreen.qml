import QtQuick 2.0
import QtQml 2.2
import QtMultimedia 5.6
import QtWebSockets 1.0
import "./resources/controls"

Item {
    id: mirrorScreen
    //property alias videoSource : mirrorScreenMediaPlayer.source
    //property alias imageSource : mirrorScreenImage.source

    QtObject {
        id:p
        property string templateName : ""
        property string effectName : ""
        property int currentPhoto : 0
        property int nb_photos : 0
        property int countdown_delay : 0
        property string current_effect: "Noir et Blanc"
    }

    state:"VIDEO_MODE"

    Rectangle {
        anchors.fill: parent
        color:"black"
        MouseArea {
            anchors.fill: parent
            onClicked: {
                //Nothing
            }
        }
    }

    MediaPlayer {
        id: mirrorScreenMediaPlayer
        autoPlay: false
    }

    VideoOutput {
        id:mirrorScreenVideoOutput
        anchors.fill: parent
        fillMode: VideoOutput.PreserveAspectFit
        opacity: 1.0
        source: mirrorScreenMediaPlayer
    }

    /*
    Image {
        id:mirrorScreenImage
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        height: parent.height * 0.8
        rotation: 5
        visible: false
        fillMode: Image.PreserveAspectFit
    }
    */
    //TODO: aspect ratio not preserved Image.PreserveAspectCrop --> Image.PreserveAspectFit
    PhotoShootRenderer {
        id:mirrorScreenImage
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        height: parent.height * 0.8
        width: height / parameters.cameraHeight * parameters.cameraWidth
        rotation: 5
        visible: false
        effectSource: p.current_effect
    }

    Text {
        id: countdown
        text: p.countdown_delay
        visible: false
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 10
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        fontSizeMode: Text.Fit; minimumPixelSize: 10; font.pixelSize: 72
        color: "white"
        width: parent.width - parent.height * 4 / 3 - 10
    }

    ListModel {
        id:mirrorScreenPhotoModel
    }

    GridView {
        id:mirrorScreenFinalResultGrid
        anchors.fill: parent
        cellWidth: count < 2 ? parent.width : count < 7 ? parent.width / 2 : parent.width / 3
        cellHeight: count < 3 ? parent.height : count < 5 ? parent.height / 2 : parent.height / 3
        visible: true
        model: mirrorScreenPhotoModel

        delegate:
            Item {
                height: mirrorScreenFinalResultGrid.cellHeight - 10
                width: mirrorScreenFinalResultGrid.cellWidth - 10

                /*
                PhotoShootRenderer {
                    anchors.centerIn: parent
                    height: parent.height * 0.95
                    width: height / parameters.cameraHeight * parameters.cameraWidth
                    visible: false
                    effectSource: p.current_effect
                    photoBaseSource: "file:///" + url
                }
                */

                Image {
                    anchors.centerIn: parent
                    height: parent.height * 0.95
                    width: parent.width * 0.95
                    source: "file:///" + url
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                }
            }
    }

    //Bouton de config
    ButtonAwesome {
        id:configButton
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 10
        anchors.rightMargin: 10
        size:parent.height * 0.10
        code:"\uf085"
        onClicked: {
            mirrorScreenMediaPlayer.stop()
            mirrorScreen.state = "CONFIG"
        }
    }

    ButtonAwesome {
        id:startPhotoProcessButton
        anchors.top: parent.top
        anchors.right: configButton.left
        anchors.topMargin: 10
        anchors.rightMargin: 20
        size:parent.height * 0.10
        code:"\uf030"
        onClicked: {
            startGlobalPhotoProcess("", "Sepia", 3);
        }
    }

    //Ecran de configuration
    ConfigScreen {
        id: configScreen
        anchors.fill: parent
        visible: false
    }

    Component.onCompleted: {
        startWaitVideo()
    }

    PhotoClientRobot {
        id:photoClientRobot
        visible: false;
    }

    states: [
        State {
            name: "VIDEO_MODE"
            PropertyChanges { target: configScreen; visible:false}
            PropertyChanges { target: mirrorScreenVideoOutput; visible: true }
            PropertyChanges { target: mirrorScreenImage; visible: false }
            PropertyChanges { target: countdown; visible: false }
            PropertyChanges { target: mirrorScreenFinalResultGrid; visible: false }
        },
        State {
            name: "PHOTO_MODE"
            PropertyChanges { target: configScreen; visible:false}
            PropertyChanges { target: mirrorScreenVideoOutput; visible: false }
            PropertyChanges { target: mirrorScreenImage; visible: false }
            PropertyChanges { target: countdown; visible: true }
            PropertyChanges { target: mirrorScreenFinalResultGrid; visible: false }
        },
        State {
            name: "PHOTO_RESULT"
            PropertyChanges { target: configScreen; visible:false}
            PropertyChanges { target: mirrorScreenVideoOutput; visible: false }
            PropertyChanges { target: mirrorScreenImage; visible: true }
            PropertyChanges { target: countdown; visible: false }
            PropertyChanges { target: mirrorScreenFinalResultGrid; visible: false }
        },
        State {
            name: "PHOTO_FINAL_RESULT"
            PropertyChanges { target: configScreen; visible:false}
            PropertyChanges { target: mirrorScreenVideoOutput; visible: false }
            PropertyChanges { target: mirrorScreenImage; visible: false }
            PropertyChanges { target: countdown; visible: false }
            PropertyChanges { target: mirrorScreenFinalResultGrid; visible: true }
        },
        State {
            name: "CONFIG"
            PropertyChanges { target: configScreen; visible:true}
            PropertyChanges { target: mirrorScreenVideoOutput; visible: false}
            PropertyChanges { target: mirrorScreenImage; visible: false}
            PropertyChanges { target: countdown; visible: false }
            PropertyChanges { target: mirrorScreenFinalResultGrid; visible: false }
        }
    ]

    //Start wait video
    function startWaitVideo() {
        mirrorScreenMediaPlayer.stop()
        mirrorScreen.state = "VIDEO_MODE"
        mirrorScreenMediaPlayer.loops = MediaPlayer.Infinite
        mirrorScreenMediaPlayer.source = "file:///" + parameters.waitVideo.videoPath
        mirrorScreenMediaPlayer.play()
    }

    //Start the wall photo process
    function startGlobalPhotoProcess(templateName, effectName, photoNumber) {
        p.templateName = templateName;
        p.effectName = effectName;
        p.nb_photos = photoNumber;
        p.currentPhoto = 0;
        mirrorScreenPhotoModel.clear()
        startPhotoProcess(0);
        mirrorScreenMediaPlayer.stop()
        if (p.currentPhoto < p.nb_photos) {
            startPhotoProcess(p.currentPhoto)
        }
    }

    //for each photo, take the photo
    function startPhotoProcess(photoNumber) {
        mirrorScreen.state = "PHOTO_MODE"
        //Start photo countdown
        p.countdown_delay = 5 //TODO : mettre ca dans la config
        //Display preview
        var camera_height = parent.height;
        var camera_width = parent.height * 4 / 3;
        var camera_y = 0;
        var camera_x = parent.width - camera_width;
        camera.startPreview("/tmp/phototwix_" + photoNumber + ".jpg", camera_x, camera_y, camera_width, camera_height);
        mirrorScreenCountdown.start();
    }

    Timer {
        id:mirrorScreenCountdown
        interval: 1000; //TODO: put that value in parameter
        running: false;
        repeat: true
        onTriggered: {
            p.countdown_delay--;
            if (p.countdown_delay <= 0) {
                mirrorScreenCountdown.stop();
                takePhoto();
            }
        }
    }

    //Take the photo
    function takePhoto(photoNumber) {
        camera.capturePhoto();
    }

    //When photo is taken
    Connections {
        target: camera
        onImageCaptured : {
            //mirrorScreenImage.source = "file:///" + filename;
            mirrorScreenImage.photoBase.source = "file:///" + filename;
            var data = {'photo_number' : p.currentPhoto,'url' : filename};
            mirrorScreenPhotoModel.append(data);
            mirrorScreen.state = "PHOTO_RESULT"
            mirrorScreenDisplayPhotoMirror.start();
        }
    }

    //Temps durant lequel on affiche la photo de résultat
    Timer {
        id:mirrorScreenDisplayPhotoMirror
        interval: 5000; //TODO: put that value in parameter
        running: false;
        repeat: false
        onTriggered: {
            p.currentPhoto++;
            if (p.currentPhoto < p.nb_photos) {
                startPhotoProcess(p.currentPhoto)
            } else {
                endGlobalPhotoProcess();
            }
        }
    }

    //End of photo process
    function endGlobalPhotoProcess() {
        console.log("End global Photo Process")
        mirrorScreen.state = "PHOTO_FINAL_RESULT"
        //TODO: Prepare JSON message to send
        if (photoClientRobot.client.status == WebSocket.Open) {
            var message= JSON.stringify({ //TODO
                  photoProcessResult:{
                    templateName: p.templateName,
                    effectName: p.effectName,
                    nbPhoto: p.nb_photos,
                    photos: [
                        {fileName:"phototwix_1.jpg",fileUrl:"/tmp/"},
                        {fileName:"phototwix_2.jpg",fileUrl:"/tmp/"},
                        {fileName:"phototwix_3.jpg",fileUrl:"/tmp/"}
                    ]
                  }
                });
            photoClientRobot.client.sendTextMessage(message);
        }


        mirrorScreenDisplayFinalResult.start()
    }

    //Temps durant lequel on affiche les photos de résultat finale
    Timer {
        id:mirrorScreenDisplayFinalResult
        interval: 15000; //TODO: put that value in parameter
        running: false;
        repeat: false
        onTriggered: {
            startWaitVideo()
        }
    }
}
