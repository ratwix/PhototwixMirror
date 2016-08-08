import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtMultimedia 5.4
import QtQuick 2.4
import QtGraphicalEffects 1.0

import "../renderer"


Item {
    id: photoShootRenderer

    property double destRotation
    property double xphoto : 0.0
    property string effectSource : globalVar.currentEffect


    Item {
        anchors.fill: parent
        id: photoResult

        Image {
            id: photoPreview
            source:(modelData.path !== "") ? modelData.path : ""
            fillMode: Image.PreserveAspectCrop
            anchors.fill: parent
            mirror: parameters.flipresult
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
        }

        EffectGrayscale {
            id:filter_black_white
            visible: effectSource === "Noir et Blanc"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectSepia {
            id:filter_sepia
            visible: effectSource === "Sepia"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectEdge {
            id:filter_edge
            visible: effectSource === "Edge"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        Effect1977 {
            id:filter_1977
            visible: effectSource === "1977"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectAmaro {
            id:filter_amaro
            visible: effectSource === "Amaro"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectBranna {
            id:filter_branna
            visible: effectSource === "Branna"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectEarlyBird {
            id:filter_earlyBird
            visible: effectSource === "Early Bird"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectHefe {
            id:filter_hefe
            visible: effectSource === "Hefe"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectHudson {
            id:filter_hudson
            visible: effectSource === "Hudson"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectInkwell {
            id:filter_inkwell
            visible: effectSource === "Inkwell"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectLomo {
            id:filter_lomo
            visible: effectSource === "Lomo"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectLordKelvin {
            id:filter_lordKelvin
            visible: effectSource === "Lord Kelvin"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectNashville {
            id:filter_nashville
            visible: effectSource === "Nashville"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectPixel {
            id:filter_pixel
            visible: effectSource == "Pixel"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectRise {
            id:filter_rise
            visible: effectSource === "Rise"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectSierra {
            id:filter_sierra
            visible: effectSource == "Sierra"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }


        EffectSutro {
            id:filter_sutro
            visible: effectSource === "Sutro"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectToaster {
            id:filter_toaster
            visible: effectSource === "Toaster"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectValancia {
            id:filter_valancia
            visible: effectSource === "Valancia"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectWalden {
            id:filter_walden
            visible: effectSource === "Walden"
            itemSource: photoPreviewShader
            anchors.fill: parent
        }

        EffectXpro {
            id:filter_xpro
            visible: effectSource === "XPro"
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
