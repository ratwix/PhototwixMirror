import QtQuick 2.0
import "."

/**
 * This is the QML input panel that provides the virtual keyboard UI
 * The code has been copied from
 * http://tolszak-dev.blogspot.de/2013/04/qplatforminputcontext-and-virtual.html
 */
Item {
    id:root
    objectName: "inputPanel"
    width: parent.width
    height: width / 5
    // Report actual keyboard rectangle to input engine
    anchors.bottom: parent.bottom

    state: "SHOW"

    signal esc()
    signal enter()

    KeyModel {
        id:keyModel
    }

    FontLoader {
        source: "../font/FontAwesome.otf"
    }

    QtObject {
        id:pimpl
        property bool shiftModifier: false
        property bool symbolModifier: false
        property int verticalSpacing: keyboard.height / 40
        property int horizontalSpacing: verticalSpacing
        property int rowHeight: keyboard.height/4 - verticalSpacing
        property int buttonWidth:  (keyboard.width-column.anchors.margins)/10 - horizontalSpacing
    }

    /**
     * The delegate that paints the key buttons
     */
    Component {
        id: keyButtonDelegate
        KeyButton {
            id: button
            width: pimpl.buttonWidth
            height: pimpl.rowHeight
            text: (pimpl.shiftModifier) ? letter.toUpperCase() : (pimpl.symbolModifier)?firstSymbol : letter
            inputPanel: root
        }
    }

    /**
     * This function shows the character preview popup for each key button
     */
    function showKeyPopup(keyButton)
    {
        keyPopup.popup(keyButton, root);
    }

    /**
     * The key popup for character preview
     */
    KeyPopup {
        id: keyPopup
        visible: false
        z: 100
    }


    Row {
        height: pimpl.rowHeight
        spacing: pimpl.horizontalSpacing
        anchors.left: keyboard.left
        anchors.bottom: keyboard.top
        KeyButton {
            id: keyboardKey
            height: pimpl.rowHeight
            color: "#1e1b18"
            width: 1.5 * pimpl.buttonWidth
            font.family: "FontAwesome"
            displayText: "\uf11c"
            inputPanel: root
            showPreview: false
            onClicked: {
                root.state = "SHOW"
            }
        }

        KeyButton {
            id: escKey
            height: pimpl.rowHeight
            color: "#1e1b18"
            width: 1.5 * pimpl.buttonWidth
            font.family: "FontAwesome"
            displayText: "Esc"
            text: "\x1B"
            functionKey: true

            inputPanel: root
            showPreview: false

            onClicked: {
                root.esc()
            }
        }
    }



    Rectangle {
        id:keyboard
        color: "black"
        anchors.top: parent.top;
        anchors.left: parent.left;
        height: parent.height;
        width: parent.width;
        MouseArea {
            anchors.fill: parent
        }

        Column {
            id:column
            anchors.margins: 5
            anchors.fill: parent
            spacing: pimpl.verticalSpacing

            Row {
                height: pimpl.rowHeight
                spacing: pimpl.horizontalSpacing
                anchors.horizontalCenter:parent.horizontalCenter
                Repeater {
                    model: keyModel.firstRowModel
                    delegate: keyButtonDelegate
                }
            }
            Row {
                height: pimpl.rowHeight
                spacing: pimpl.horizontalSpacing
                anchors.horizontalCenter:parent.horizontalCenter
                Repeater {
                    model: keyModel.secondRowModel
                    delegate: keyButtonDelegate
                }
            }
            Item {
                height: pimpl.rowHeight
                width:parent.width
                KeyButton {
                    id: shiftKey
                    color: (pimpl.shiftModifier)? "#1e6fa7": "#1e1b18"
                    anchors.left: parent.left
                    width: 1.25*pimpl.buttonWidth
                    height: pimpl.rowHeight
                    font.family: "FontAwesome"
                    text: "\uf062"
                    functionKey: true
                    onClicked: {
                        if (pimpl.symbolModifier) {
                            pimpl.symbolModifier = false
                        }
                        pimpl.shiftModifier = !pimpl.shiftModifier
                    }
                    inputPanel: root
                }
                Row {
                    height: pimpl.rowHeight
                    spacing: pimpl.horizontalSpacing
                    anchors.horizontalCenter:parent.horizontalCenter
                    Repeater {
                        anchors.horizontalCenter: parent.horizontalCenter
                        model: keyModel.thirdRowModel
                        delegate: keyButtonDelegate
                    }
                }
                KeyButton {
                    id: backspaceKey
                    font.family: "FontAwesome"
                    color: "#1e1b18"
                    anchors.right: parent.right
                    width: 1.25*pimpl.buttonWidth
                    height: pimpl.rowHeight
                    text: "\x7F"
                    displayText: "\uf177"
                    inputPanel: root
                    repeat: true
                }
            }
            Row {
                height: pimpl.rowHeight
                spacing: pimpl.horizontalSpacing
                anchors.horizontalCenter:parent.horizontalCenter
                KeyButton {
                    id: hideKey
                    color: "#1e1b18"
                    width: 1.25*pimpl.buttonWidth
                    height: pimpl.rowHeight
                    font.family: "FontAwesome"
                    text: "\uf078"
                    functionKey: true
                    onClicked: {
                        root.state = "HIDE"
                    }
                    inputPanel: root
                    showPreview: false
                }
                KeyButton {
                    color: "#1e1b18"
                    width: 1.25*pimpl.buttonWidth
                    height: pimpl.rowHeight
                    text: ""
                    inputPanel: root
                    functionKey: true
                }
                KeyButton {
                    width: pimpl.buttonWidth
                    height: pimpl.rowHeight
                    text: ","
                    //onClicked: {}//TODO: InputEngine.sendKeyToFocusItem(text)
                    inputPanel: root
                }
                KeyButton {
                    id: spaceKey
                    width: 3*pimpl.buttonWidth
                    height: pimpl.rowHeight
                    text: " "
                    inputPanel: root
                    showPreview: false
                }
                KeyButton {
                    width: pimpl.buttonWidth
                    height: pimpl.rowHeight
                    text: "."
                    inputPanel: root
                }
                KeyButton {
                    id: symbolKey
                    color: "#1e1b18"
                    width: 1.25*pimpl.buttonWidth
                    height: pimpl.rowHeight
                    text: (!pimpl.symbolModifier)? "12#" : "ABC"
                    functionKey: true
                    onClicked: {
                        if (pimpl.shiftModifier) {
                            pimpl.shiftModifier = false
                        }
                        pimpl.symbolModifier = !pimpl.symbolModifier
                    }
                    inputPanel: root
                }
                KeyButton {
                    id: enterKey
                    color: "#1e1b18"
                    width: 1.25*pimpl.buttonWidth
                    height: pimpl.rowHeight
                    displayText: "Enter"
                    text: "\n"
                    inputPanel: root
                    onPressed: {
                        root.enter()
                    }
                }
            }
        }
    }

    states: [
        State {
            name: "SHOW"
        },

        State {
            name: "HIDE"
            PropertyChanges { target: keyboard; anchors.top:root.bottom}
            PropertyChanges { target: escKey; visible:false}
        },

        State {
            name: "HIDE_TOTAL"
            PropertyChanges { target: root; visible: false}
        }
    ]
}
