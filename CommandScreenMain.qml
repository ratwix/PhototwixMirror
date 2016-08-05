import QtQuick 2.6
import QtQuick.Window 2.2

import com.phototwix.components 1.0
import "./resources/controls"

Window {
    title: "Phototwix V5"
    visible: true
    height: 800
    width: 1200
    
    QtObject {
        id:globalVar
        property color      backColor:               "#6C6F70"
        property color      backColorTemplate:       "#005A8C"
        property color      backColorTemplateLast:   "#F2AF32"
        property Template   currentTemplate;
        property string     currentEffect;

        onCurrentTemplateChanged: {
            console.debug("CTemplate:" + currentTemplate.name)
            console.debug("CTemplateTwitter:" + parameters.twitterDefaultTemplate.name)
        }

        onCurrentEffectChanged: {
            console.debug("CEffect:" + currentEffect)
            console.debug("CEffectChange:" + parameters.effectDefault)
        }
    }

    CommandScreen {
        anchors.fill: parent
    }

}
