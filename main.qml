import QtQuick 2.6
import QtQuick.Window 2.2

import com.phototwix.components 1.0

Window {
    title: "Phototwix V5"
    visible: true
    height: 900
    width: 1600
    
    Item {
        id: mainItem
        anchors.fill: parent

        ConfigScreen {
            id: configScreen
            anchors.fill: parent
        }
    }
}
