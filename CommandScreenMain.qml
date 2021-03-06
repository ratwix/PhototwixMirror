import QtQuick 2.6
import QtQuick.Window 2.2

import com.phototwix.components 1.0
import "./resources/controls"

Window {
    title: "Phototwix V5 Command"
    visible: true
    visibility: Window.FullScreen

    //height: 480
    //width: 800
    /*
    height: 800
    width: 1200
    */
    QtObject {
        id:globalVar
        property color      backColor:               "#6C6F70"
        property color      backColorTemplate:       "#005A8C"
        property color      backColorTemplateLast:   "#F2AF32"
        property Template   currentTemplate;
        property string     currentEffect;
    }

    FontLoader {
        source: "resources/font/Gabrielle.ttf"
    }

    FontLoader {
        source: "resources/font/FontAwesome.otf"
    }

    FontLoader {
        source: "resources/font/arista.ttf"
    }

    CommandScreen {
        anchors.fill: parent
    }

    Component.onCompleted: {
      console.debug("Screen height:" + height + " width:" + width)
    }

}
