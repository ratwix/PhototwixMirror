import QtQuick 2.0
import QtQml 2.2
import QtMultimedia 5.6

Item {
    id: mirrorScreen
    property alias videoSource : mirrorScreenMediaPlayer.source
    property alias imageSource : mirrorScreenImage.source

    QtObject {
        id:p
        property int currentPhoto : 0
        property int nb_photos : 4 //TODO:photoPartModel.count
    }

    state:"VIDEO_MODE"

    MediaPlayer {
        id: mirrorScreenMediaPlayer
    }

    VideoOutput {
        id:mirrorScreenVideoOutput
        anchors.fill: parent
        fillMode: VideoOutput.PreserveAspectFit
        opacity: 1.0
        source: mediaplayer
    }

    Image {
        id:mirrorScreenImage
        anchors.fill: parent
        opacity: 0.0
        fillMode: Image.PreserveAspectFit
    }

    Connections {
        target: cameraWorker

        onImageCaptured: {
            //Image captured, display image and start countdown to continue
            var path = fileName
            mirrorScreenImage.source = path
            //TODO: add image to template : applicationWindows.currentPhoto.photoPartList[p.currentPhoto].pathS = path;
            mirrorScreenDisplayPhotoMirror.start();
        }

        onImageCaptureError: {
            endGlobalPhotoProcessAfterError();
        }
    }

    Timer {
        id:mirrorScreenDisplayPhotoMirror
        interval: 3000; //TODO: put that value in parameter
        running: false;
        repeat: false
        onTriggered: {
            //if there is more photo to take
        }
    }


    states: [
        State {
            name: "VIDEO_MODE"
        },
        State {
            name: "PHOTO_MODE"
            PropertyChanges { target: mirrorScreenVideoOutput
                              opacity: 0.0
            }
            PropertyChanges { target: mirrorScreenImage
                              opacity: 1.0
            }
        }
    ]

    //Start the wall photo process
    function startGlobalPhotoProcess() {
        //Play start photo process video
    }

    //for each photo, take the photo
    function startPhotoProcess(photoNumber) {

    }

    //for each photo, take the photo
    function takePhoto() {

    }

    //Display the photo result
    function displayPhotoResult() {

    }

    //End of photo process
    function endGlobalPhotoProcess() {

    }

    //End of photo process when error
    function endGlobalPhotoProcessOnError() {

    }
}
