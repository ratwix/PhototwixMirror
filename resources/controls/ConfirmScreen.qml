import QtQuick 2.0

Rectangle {
    id:confirmScreen
    anchors.fill: parent
    property string message: "message"
    property bool   show: false
    property var acceptFunction: function(){}
    property var refuseFunction: function(){}
    color:"#C0212126"

    signal accept
    signal refuse

    state:"hide"
    opacity: 0.0
    visible: opacity != 0.0

    MouseArea {
        anchors.fill: parent
    }

    Text {
        width: parent.width
        height: parent.height * 0.4
        anchors.top:parent.top
        anchors.topMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter
        fontSizeMode :  Text.Fit
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: "white"
        text:message
        minimumPixelSize: 30
        font.pixelSize: 80
    }

    Image {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.bottomMargin: 40
        anchors.leftMargin: 40
        fillMode: Image.PreserveAspectFit
        height: parent.height * 0.4
        source: "../images/Refuse-icon.png"
        MouseArea {
            anchors.fill: parent
            onClicked: {
                confirmScreen.refuse()
                refuseFunction()
                confirmScreen.state="hide"
            }
        }
    }

    Image {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.bottomMargin: 40
        anchors.rightMargin: 40
        fillMode: Image.PreserveAspectFit
        height: parent.height * 0.4
        source: "../images/Accept-icon.png"
        MouseArea {
            anchors.fill: parent
            onClicked: {
                confirmScreen.accept()
                confirmScreen.acceptFunction()
                confirmScreen.state="hide"
            }
        }
    }

    states: [
        State {
            name: "hide"
        },
        State {
            name: "show"
            PropertyChanges { target: confirmScreen; opacity: 1.0}
        }

    ]

    transitions: [
        Transition {
            from: 'hide'; to: 'show'
            NumberAnimation {
                target: confirmScreen; properties:'opacity'; duration: 400; easing.type: 'OutQuart'
            }
        }
    ]

    onStateChanged: {
        if (state == "show") {

        }
    }
}

