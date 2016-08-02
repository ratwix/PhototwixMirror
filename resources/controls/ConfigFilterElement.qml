import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import '../renderer'

Item {
    property url    effectImageSource: ""
    property string effectName : ""
    property alias  effectActive: effectActiveSwitch.checked
    property alias  effectTwitterDefault : effectTwitterDefault.checked

    Row {
        spacing : 30
        anchors.leftMargin: 20
        height: parent.height
        anchors.left: parent.left

        Item {
            height: parent.height
            width: parent.height * 1.5
            Image {
                    id:effectButtonImage
                    source: "../images/samplePhotobooth.jpg"
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
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
        }

        Grid {
                height: parent.height
                columns: 2
                columnSpacing: 10
                anchors.verticalCenter: parent.verticalCenter

                Label {
                    height: 30
                    text: "Active"
                    font.pixelSize: 15
                }

                Switch {
                    id:effectActiveSwitch
                    onCheckedChanged: {
                        model.modelData.effectEnable = checked;
                        parameters.Serialize();
                    }
                    Component.onCompleted: {
                        checked = model.modelData.effectEnable
                    }
                }

                Label {
                    height: 30
                    text: "Twitter Default"
                    font.pixelSize: 15
                }

                RadioButton {
                    id:effectTwitterDefault
                    text: "Select"
                    exclusiveGroup: effectTwitterDefaultGroup
                    onCheckedChanged: {
                        model.modelData.effectTwitterDefault = checked
                        parameters.Serialize();
                    }
                    Component.onCompleted: {
                        checked = model.modelData.effectTwitterDefault
                    }
                }
        }
    }
}
