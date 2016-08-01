import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    property variant itemSource
    property real    value:1.0

    Colorize {
        anchors.fill: parent
        source: itemSource
        hue: 0.1
        saturation: 0
        lightness: 0.15
        opacity: value
    }
}
