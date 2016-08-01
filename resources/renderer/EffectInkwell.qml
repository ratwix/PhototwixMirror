import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    property variant itemSource

    Rectangle {
        anchors.fill: parent
        color: "white"
    }

    Image {
        id:inkwellMapImg
        visible: false
        source: "../filtersResources/inkwellMap.png"
    }

    ShaderEffectSource {
        id: inkwellMap
        sourceItem: inkwellMapImg
    }

    ShaderEffect {
        id:inkwellFilter
        anchors.fill: parent

        property variant inputImageTexture: itemSource
        property variant inputImageTexture2: inkwellMap

        fragmentShader: "
             varying highp vec2 qt_TexCoord0;

             uniform sampler2D inputImageTexture;
             uniform sampler2D inputImageTexture2;

             void main()
             {
                 vec3 texel = texture2D(inputImageTexture, qt_TexCoord0).rgb;
                 texel = vec3(dot(vec3(0.3, 0.6, 0.1), texel));
                 texel = vec3(texture2D(inputImageTexture2, vec2(texel.r, .16666)).r);
                 gl_FragColor = vec4(texel, 1.0);
             }"
    }
}
