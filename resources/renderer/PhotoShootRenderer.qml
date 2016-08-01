import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtMultimedia 5.4
import QtQuick 2.4
import QtGraphicalEffects 1.0


Flipable {
    id: photoShootRenderer

    property bool flipped: false
    property int  photoIndex: 0
    property bool photoTaked: false
    property double destRotation
    property double xphoto : 0.0
    property string effectSource : applicationWindows.effectSource

    signal processEnd()

    function startPhotoProcess() {
        flipped = true;
        photoTaked = true;
        destRotation = -rotation;
        resetFilters();
    }

    function endPhotoProcess(photoResult) {
        photoPreview.source = "file:///" + photoResult;
    }

    function resetFilters() {
        applicationWindows.effectSource = "Couleur"
    }

    front: Rectangle {
        anchors.fill: parent
        id: photoResult
        color:"#8097AC"

        Image {
            id: photoPreview
            source:(modelData.path != "") ? "file:///" + modelData.path : ""
            anchors.fill: parent
            mirror: parameters.flipresult
            smooth: true
            onStatusChanged: {
                if ((source != "") && (photoPreview.status == Image.Ready)) {
                    photoTaked = true; //photo is taked, wait to switch finish
                    flipped = false;
                    currentLabel.visible = false;

                }
            }
        }

        ShaderEffectSource { //Duplication de la camera
            id:photoPreviewShader
            anchors.fill: parent
            sourceItem: photoPreview
            hideSource: true
            sourceRect:Qt.rect(photoPreview.width * xphoto,
                               0,
                               photoPreview.width - photoPreview.width * xphoto * 2,
                               photoPreview.height);
        }

        EffectGrayscale {
            id:filter_black_white
            visible: applicationWindows.effectSource == "Noir et Blanc"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectSepia {
            id:filter_sepia
            visible: applicationWindows.effectSource == "Sepia"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectEdge {
            id:filter_edge
            visible: applicationWindows.effectSource == "Edge"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        Effect1977 {
            id:filter_1977
            visible: applicationWindows.effectSource == "1977"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectAmaro {
            id:filter_amaro
            visible: applicationWindows.effectSource == "Amaro"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectBranna {
            id:filter_branna
            visible: applicationWindows.effectSource == "Branna"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectEarlyBird {
            id:filter_earlyBird
            visible: applicationWindows.effectSource == "Early Bird"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectHefe {
            id:filter_hefe
            visible: applicationWindows.effectSource == "Hefe"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectHudson {
            id:filter_hudson
            visible: applicationWindows.effectSource == "Hudson"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectInkwell {
            id:filter_inkwell
            visible: applicationWindows.effectSource == "Inkwell"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectLomo {
            id:filter_lomo
            visible: applicationWindows.effectSource == "Lomo"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectLordKelvin {
            id:filter_lordKelvin
            visible: applicationWindows.effectSource == "Lord Kelvin"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectNashville {
            id:filter_nashville
            visible: applicationWindows.effectSource == "Nashville"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectPixel {
            id:filter_pixel
            visible: applicationWindows.effectSource == "Pixel"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectRise {
            id:filter_rise
            visible: applicationWindows.effectSource == "Rise"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectSierra {
            id:filter_sierra
            visible: applicationWindows.effectSource == "Sierra"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }


        EffectSutro {
            id:filter_sutro
            visible: applicationWindows.effectSource == "Sutro"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectToaster {
            id:filter_toaster
            visible: applicationWindows.effectSource == "Toaster"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectValancia {
            id:filter_valancia
            visible: applicationWindows.effectSource == "Valancia"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectWalden {
            id:filter_walden
            visible: applicationWindows.effectSource == "Walden"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectXpro {
            id:filter_xpro
            visible: applicationWindows.effectSource == "XPro"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }


        Text {
            id:currentLabel
            anchors.centerIn: parent
            font.pixelSize: parent.height * 0.6
            text: photoIndex
        }
    }


    back: Rectangle {
        anchors.fill: parent
        id:videoPreviewArea
        color:"orange"

        ShaderEffectSource { //Duplication de la camera
            id:videoPreview
            anchors.top: parent.top
            anchors.left: parent.left
            height: parent.height
            width: parent.width
            sourceItem: cameraVideoOutput
            sourceRect:Qt.rect(cameraVideoOutput.width * xphoto,
                               0,
                               cameraVideoOutput.width - cameraVideoOutput.width * xphoto * 2,
                               cameraVideoOutput.height);
            live:true
            transform: Rotation { origin.x: videoPreview.width / 2; axis { x: 0; y: 1; z: 0 } angle: parameters.flipcamera ? 180 : 0 }
        }
    }

    transform: Rotation {
        id: rotationTransformer
        origin.x: photoShootRenderer.width/2
        origin.y: photoShootRenderer.height/2
        axis.x: 0; axis.y: 1; axis.z: 0     // set axis.y to 1 to rotate around y-axis
        angle: 0    // the default angle
    }

    states: State {
        name: "back"
        PropertyChanges { target: rotationTransformer; angle: 180 }
        PropertyChanges { target: photoShootRenderer; rotation: destRotation}
        when: photoShootRenderer.flipped
    }

    transitions: Transition {
        ParallelAnimation {
            NumberAnimation { target: rotationTransformer; property: "angle"; duration: 400 }
            NumberAnimation { target: photoShootRenderer; property: "rotation"; duration: 400 }
            SequentialAnimation {
                NumberAnimation { target: photoShootRenderer; property: "scale"; to: 0.8; duration: 200 }
                NumberAnimation { target: photoShootRenderer; property: "scale"; to: 1.0; duration: 200 }
            }
        }
        onRunningChanged: {
            if ((flipped == false) && !running && photoTaked) {
                photoShootRenderer.processEnd() //tell that process is end
            }
        }
    }
}
