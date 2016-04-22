import QtQuick 2.0

Rectangle {
    id:escapeScreen
    anchors.fill: parent
    property string password: "photo123"
    property string passwordAdmin: "pm"
    property bool   show: false
    color:"#C0212126"

    state:"hide"
    opacity: 0.0
    visible: opacity != 0.0

    signal success()
    signal successAdmin()
    signal failed()

    FontLoader {
        source: "../font/FontAwesome.otf"
    }


    MouseArea {
        anchors.fill: parent
    }


    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top : parent.top
        anchors.topMargin: 20
        height: parent.height * 0.3
        font.pixelSize: 200
        color:"white"
        font.family: "FontAwesome"
        text: "\uf023"
    }

    Text {
        id:textLabel
        width: 300
        //height: parent.height * 0.4
        anchors.top: parent.top
        anchors.topMargin: 250
        anchors.horizontalCenter: parent.horizontalCenter
        fontSizeMode :  Text.Fit
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: "white"
        text:"Mot de passe :"
        minimumPixelSize: 30
        font.pixelSize: 80
    }

    TextInput {
        id:passInput
        width: 400
        height: textLabel.height
        anchors.left: textLabel.right
        anchors.top: textLabel.top
        anchors.leftMargin: 30
        echoMode: TextInput.Password
        color: "white"
        font.pixelSize: textLabel.font.pixelSize
    }



    onStateChanged: {
        if (state == "show") {
            passInput.forceActiveFocus()
        }
    }

    InputPanel {
        id:keyboard
        state:"SHOW"
        onEsc: {
            escapeScreen.state = "hide"
        }
        onEnter: {
            if (passInput.text == password) {
                escapeScreen.state = "hide"
                passInput.text = ""
                escapeScreen.success()
            } else {
                if (passInput.text == passwordAdmin) {
                    escapeScreen.state = "hide"
                    passInput.text = ""
                    escapeScreen.successAdmin()
                } else {
                    passInput.text = ""
                    escapeScreen.failed()
                }
            }
        }
    }

    states: [
        State {
            name: "hide"
        },
        State {
            name: "show"
            PropertyChanges { target: escapeScreen; opacity: 1.0}
        }

    ]

    transitions: [
        Transition {
            from: 'hide'; to: 'show'
            NumberAnimation {
                target: escapeScreen; properties:'opacity'; duration: 400; easing.type: 'OutQuart'
            }
        }
    ]
}
