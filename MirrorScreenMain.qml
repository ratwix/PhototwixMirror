import QtQuick 2.0
import QtQuick.Window 2.2

import com.phototwix.components 1.0

Window {
    title: "Phototwix V5 Mirror"
    visible: true
    visibility: Window.FullScreen


    QtObject {
        id:globalVar
        property color      backColor:               "#6C6F70"
        property color      backColorTemplate:       "#005A8C"
        property color      backColorTemplateLast:   "#F2AF32"
        property Template   currentTemplate;
        property string     currentEffect;
        property int        screenHeight:            height;
        property int        screenWidth:             width;
    }

    FontLoader {
        source: "resources/font/Gabrielle.ttf"
    }

    FontLoader {
        source: "resources/font/FontAwesome.otf"
    }

    MirrorScreen {
        anchors.fill: parent
    }

    Component.onCompleted: {
      console.debug("Screen height:" + height + " width:" + width)
    }

}
