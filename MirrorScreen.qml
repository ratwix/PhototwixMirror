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
        property string current_effect: "Couleur"
        property string current_video_stage: "wait"
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
        onStopped: {
            var stage = p.current_video_stage
            if (p.current_video_stage == "wait") {
                mirrorScreenMediaPlayer.seek(0);
                mirrorScreenMediaPlayer.play();
            }
            if (p.current_video_stage == "intro_play") {
                startPhotoProcess(p.currentPhoto);
            }
            if (p.current_video_stage == "outro_play") {
                startWaitVideo();
            }
        }

        onStatusChanged: {
            if (status == MediaPlayer.Loaded) {
                if (p.current_video_stage == "intro") {
                    mirrorScreenMediaPlayer.play();
                    p.current_video_stage = "intro_play"
                }

                if (p.current_video_stage == "outro") {
                    mirrorScreenMediaPlayer.play();
                    p.current_video_stage = "outro_play"
                }
            }
            if (status == MediaPlayer.InvalidMedia) {
                console.error("Invalid video format");
                startWaitVideo();
            }
        }
    }

    VideoOutput {
        id:mirrorScreenVideoOutput
        anchors.fill: parent
        fillMode: VideoOutput.PreserveAspectFit
        opacity: 1.0
        source: mirrorScreenMediaPlayer
    }


    Image {
        id:mirrorScreenImage
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        height: parent.height * 0.8
        rotation: 5
        visible: false
        fillMode: Image.PreserveAspectFit
        antialiasing: true
    }

    //TODO: aspect ratio not preserved Image.PreserveAspectCrop --> Image.PreserveAspectFit
    /*
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
    */

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
            //var message = '{"startGlobalPhotoProcess":{"effectName":"Couleur","nbPhoto":1,"templateName":"lucie_1.png"}}';
            //var message = '{"startGlobalPhotoProcess":{"effectName":"Sepia","nbPhoto":3,"templateName":"lucie_2.png"}}';
            var message = '{"startGlobalPhotoProcess":{"effectName":"Inkwell","nbPhoto":3,"templateName":"lucie_3.png"}}';
            receiveMessage(message);
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
        p.current_video_stage = "wait"
        mirrorScreen.state = "VIDEO_MODE"
        mirrorScreenMediaPlayer.source = "file:///" + parameters.waitVideo.videoPath
        mirrorScreenMediaPlayer.play()
    }

    function receiveMessage(message) {
        var json = JSON.parse(message);
        var templateName = ""
        var effectName = ""
        var photoNumber = ""
        if (json.startGlobalPhotoProcess) {
            templateName = json.startGlobalPhotoProcess.templateName;
            effectName = json.startGlobalPhotoProcess.effectName;
            photoNumber = json.startGlobalPhotoProcess.nbPhoto;
        } else {
            console.error("JSON didn't contain startGlobalPhotoProcess")
        }

        startGlobalPhotoProcess(templateName, effectName, photoNumber);
    }

    //Start the wall photo process
    function startGlobalPhotoProcess(templateName, effectName, photoNumber) {
        p.templateName = templateName;
        p.effectName = effectName;
        p.nb_photos = photoNumber;
        p.currentPhoto = 0;
        startIntroVideo()

    }

    function startIntroVideo() {
        mirrorScreen.state = "VIDEO_MODE"
        mirrorScreenMediaPlayer.stop()
        mirrorScreenMediaPlayer.source = "file:///" + parameters.startGlobalPhotoProcessVideo.videoPath
        mirrorScreenMediaPlayer.play()
        p.current_video_stage = "intro"
    }

    //for each photo, take the photo
    function startPhotoProcess(photoNumber) {
        mirrorScreenMediaPlayer.stop()
        mirrorScreen.state = "PHOTO_MODE"
        //Start photo countdown
        p.countdown_delay = parameters.countdown
        //Display preview
        var camera_height = parent.height;
        var camera_width = parent.height * 4 / 3;
        var camera_y = 0;
        var camera_x = parent.width - camera_width;
        var date = new Date();
        var n = date.getFullYear() + "_" + (date.getMonth() + 1) + "_" + date.getDate() + "_" + date.getHours() + "h" + date.getMinutes() + "m" + date.getSeconds() + "s";
        camera.startPreview("/tmp/phototwix_" + n + "_" + photoNumber + ".jpg", camera_x, camera_y, camera_width, camera_height);
        mirrorScreenCountdown.start();
    }

    Timer {
        id:mirrorScreenCountdown
        interval: 1000;
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
            mirrorScreenImage.source = "file:///" + filename;
            //mirrorScreenImage.photoBase.source = "file:///" + filename;
            var data = {'photo_number' : p.currentPhoto,'url' : filename};
            mirrorScreenPhotoModel.append(data);
            mirrorScreen.state = "PHOTO_RESULT"
            mirrorScreenDisplayPhotoMirror.start();
        }
    }

    //Temps durant lequel on affiche la photo de résultat
    Timer {
        id:mirrorScreenDisplayPhotoMirror
        interval: parameters.viewPhotoTime * 1000
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
        if (p.nb_photos <= 1) {
            mirrorScreenDisplayFinalResult.interval = 10
        } else {
            mirrorScreenDisplayFinalResult.interval = parameters.viewAllPhotoTime * 1000
        }

        mirrorScreen.state = "PHOTO_FINAL_RESULT"

        var message_json= { //TODO
              photoProcessResult:{
                templateName: p.templateName,
                effectName: p.effectName,
                nbPhoto: p.nb_photos,
                photos: []
              }
        };

        for (var i = 0; i < p.nb_photos; i++) {
            var fileNumber = mirrorScreenPhotoModel.get(i).photo_number;
            var fileUrl = mirrorScreenPhotoModel.get(i).url;
            var file = fileUrl.substring(fileUrl.lastIndexOf('/')+1);
            var dir = "/tmp/";
            message_json.photoProcessResult.photos.push({
                fileName: file,
                fileUrl: dir,
                number: fileNumber
            });
        }

        var message = JSON.stringify(message_json);
        console.debug(message);

        //TODO: Prepare JSON message to send
        if (photoClientRobot.client.status == WebSocket.Open) {
            photoClientRobot.client.sendTextMessage(message);
        }
        mirrorScreenDisplayFinalResult.start()
    }

    //Temps durant lequel on affiche les photos de résultat finale
    Timer {
        id:mirrorScreenDisplayFinalResult
        interval: parameters.viewAllPhotoTime * 1000
        running: false;
        repeat: false
        onTriggered: {
            startOutroVideo()
        }
    }

    function startOutroVideo() {
        mirrorScreen.state = "VIDEO_MODE"
        mirrorScreenMediaPlayer.source = "file:///" + parameters.endGlobalPhotoProcessVideo.videoPath
        mirrorScreenMediaPlayer.play()
        p.current_video_stage = "outro"
    }
}
