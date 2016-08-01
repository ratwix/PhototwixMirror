import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

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

        Image {
                id:effectImage
                source: "../images/samplePhotobooth.jpg"
                height: parent.height
                fillMode: Image.PreserveAspectFit

                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    text: effectName
                }
        }

        Grid {
                height: parent.height
                columns: 2
                columnSpacing: 10
                Label {
                    height: 30
                    text: "Active"
                    font.pixelSize: 15
                }

                Switch {
                    id:effectActiveSwitch
                    onCheckedChanged: {
                        //model.modelData.active = checked
                    }
                    Component.onCompleted: {
                        //checked = model.modelData.active
                    }
                }

                Label {
                    height: 30
                    text: "Twitter Default"
                    font.pixelSize: 15
                }

                Switch {
                    id:effectTwitterDefault
                    onCheckedChanged: {
                        //model.modelData.twitterDefault = checked
                    }
                    Component.onCompleted: {
                        //checked = model.modelData.twitterDefault
                    }
                }
        }
    }
}