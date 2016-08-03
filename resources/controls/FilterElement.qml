import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import '../renderer'

Item {
    id:filterElement
    property string effectName : ""
    property bool  effectTwitterDefault : false


        Image {
                id:effectButtonImage
                source: "../images/samplePhotobooth.jpg"
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                visible:effectName == "Couleur"
        }

        ShaderEffectSource { //Duplication de la camera
                id:shaderImageSource
                anchors.fill: parent
                sourceItem: effectButtonImage
                visible: false
        }

        EffectSepia {
                anchors.fill: parent
                itemSource:shaderImageSource
                visible:effectName == "Sepia"
        }

        EffectEdge {
                anchors.fill: parent
                itemSource:shaderImageSource
                visible:effectName == "Edge"
        }

        EffectInkwell {
            anchors.fill: parent
            itemSource:shaderImageSource
            visible:effectName == "Inkwell"
        }

        Effect1977 {
            anchors.fill: parent
            itemSource:shaderImageSource
            visible:effectName == "1977"
        }

        EffectAmaro {
            anchors.fill: parent
            itemSource:shaderImageSource
            visible:effectName == "Amaro"
        }

        EffectBranna {
            anchors.fill: parent
            itemSource:shaderImageSource
            visible:effectName == "Branna"
        }

        EffectEarlyBird {
            anchors.fill: parent
            itemSource:shaderImageSource
            visible:effectName == "Early Bird"
        }

        EffectHefe {
            anchors.fill: parent
            itemSource:shaderImageSource
            visible:effectName == "Hefe"
        }

        EffectHudson {
            anchors.fill: parent
            itemSource:shaderImageSource
            visible:effectName == "Hudson"
        }

        EffectLomo {
            anchors.fill: parent
            itemSource:shaderImageSource
            visible:effectName == "Lomo"
        }

        EffectLordKelvin {
            anchors.fill: parent
            itemSource:shaderImageSource
            visible:effectName == "Lord Kelvin"
        }

        EffectNashville {
            anchors.fill: parent
            itemSource:shaderImageSource
            visible:effectName == "Nashville"
        }

        EffectPixel {
            anchors.fill: parent
            itemSource:shaderImageSource
            visible:effectName == "Pixel"
        }

        EffectRise {
            anchors.fill: parent
            itemSource:shaderImageSource
            visible:effectName == "Rise"
        }

        EffectSierra {
            anchors.fill: parent
            itemSource:shaderImageSource
            visible:effectName == "Sierra"
        }

        EffectSutro {
            anchors.fill: parent
            itemSource:shaderImageSource
            visible:effectName == "Sutro"
        }

        EffectToaster {
            anchors.fill: parent
            itemSource:shaderImageSource
            visible:effectName == "Toaster"
        }

        EffectValancia {
            anchors.fill: parent
            itemSource:shaderImageSource
            visible:effectName == "Valancia"
        }

        EffectWalden {
            anchors.fill: parent
            itemSource:shaderImageSource
            visible:effectName == "Walden"
        }

        EffectXpro {
            anchors.fill: parent
            itemSource:shaderImageSource
            visible:effectName == "XPro"
        }

        Rectangle {
            border.color: "white"
            border.width: 1
            color: "#A0707070"
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height * 0.20
            width: parent.width * 0.60

            Text {
                id: effectNameLabel
                text: qsTr(effectName)
                font.pixelSize: parent.height * 0.6
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                color: "white"
            }
        }

        Text {
            id:effectSelectTwitterDefaultIcon
            color: "white"
            height: parent.height * 0.1
            width: effectSelectTwitterDefaultIcon.height
            anchors.top : effectButtonImage.top
            anchors.right: effectButtonImage.right
            fontSizeMode: Text.Fit
            font.pixelSize: height
            font.family: "FontAwesome"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: "\uf081"
            visible: effectTwitterDefault
        }
}
