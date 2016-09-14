import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick 2.4

import "../renderer"


Item {
    id: photoShootRenderer

    property double destRotation
    property double xphoto : 0.0
    property string effectSource : globalVar.currentEffect
    property alias photoBase: photoPreview
    property alias photoBaseSource: photoPreview.source


    Item {
        anchors.fill: parent
        id: photoResult

        Image {
            id: photoPreview
            source:modelData.path
            fillMode: Image.PreserveAspectCrop
            anchors.fill: parent
            mirror: parameters.flipresult
            cache: false
            asynchronous: false
            smooth: true
        }

        ShaderEffectSource { //Duplication du resultat dans un shader
            id:photoPreviewShader
            anchors.fill: parent
            sourceItem: photoPreview
            hideSource: true
            sourceRect:Qt.rect(photoPreview.width * xphoto,
                               0,
                               photoPreview.width - photoPreview.width * xphoto * 2,
                               photoPreview.height);
            visible: photoPreview.status === Image.Ready
        }

        EffectGrayscale {
            id:filter_black_white
            visible: photoPreview.status === Image.Ready && effectSource === "Noir et Blanc"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectSepia {
            id:filter_sepia
            visible: photoPreview.status === Image.Ready && effectSource === "Sepia"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectEdge {
            id:filter_edge
            visible: photoPreview.status === Image.Ready && effectSource === "Edge"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        Effect1977 {
            id:filter_1977
            visible: photoPreview.status === Image.Ready && effectSource === "1977"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectAmaro {
            id:filter_amaro
            visible: photoPreview.status === Image.Ready && effectSource === "Amaro"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectBranna {
            id:filter_branna
            visible: photoPreview.status === Image.Ready && effectSource === "Branna"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectEarlyBird {
            id:filter_earlyBird
            visible: photoPreview.status === Image.Ready && effectSource === "Early Bird"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectHefe {
            id:filter_hefe
            visible: photoPreview.status === Image.Ready && effectSource === "Hefe"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectHudson {
            id:filter_hudson
            visible: photoPreview.status === Image.Ready && effectSource === "Hudson"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectInkwell {
            id:filter_inkwell
            visible: photoPreview.status === Image.Ready && effectSource === "Inkwell"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectLomo {
            id:filter_lomo
            visible: photoPreview.status === Image.Ready && effectSource === "Lomo"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectLordKelvin {
            id:filter_lordKelvin
            visible: photoPreview.status === Image.Ready && effectSource === "Lord Kelvin"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectNashville {
            id:filter_nashville
            visible: photoPreview.status === Image.Ready && effectSource === "Nashville"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectPixel {
            id:filter_pixel
            visible: photoPreview.status === Image.Ready && effectSource == "Pixel"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectRise {
            id:filter_rise
            visible: photoPreview.status === Image.Ready && effectSource === "Rise"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectSierra {
            id:filter_sierra
            visible: photoPreview.status === Image.Ready && effectSource == "Sierra"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }


        EffectSutro {
            id:filter_sutro
            visible: photoPreview.status === Image.Ready && effectSource === "Sutro"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectToaster {
            id:filter_toaster
            visible: photoPreview.status === Image.Ready && effectSource === "Toaster"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectValancia {
            id:filter_valancia
            visible: photoPreview.status === Image.Ready && effectSource === "Valancia"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectWalden {
            id:filter_walden
            visible: photoPreview.status === Image.Ready && effectSource === "Walden"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectXpro {
            id:filter_xpro
            visible: photoPreview.status === Image.Ready && effectSource === "XPro"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }
    }



    transform: Rotation {
        id: rotationTransformer
        origin.x: photoShootRenderer.width/2
        origin.y: photoShootRenderer.height/2
        axis.x: 0; axis.y: 1; axis.z: 0     // set axis.y to 1 to rotate around y-axis
        angle: 0    // the default angle
    }
}
