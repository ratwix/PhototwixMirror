import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    property variant itemSource

    Rectangle {
        anchors.fill: parent
        color: "white"
    }

    Image {
        id:mapThousandSevenImg
        visible: false
        source: "../filtersResources/1977map.png"
    }

    ShaderEffectSource {
        id: mapThousandSeven
        sourceItem: mapThousandSevenImg
    }

    Image {
        id:blowoutThousandSevenImg
        visible: false
        source: "../filtersResources/1977blowout.png"
    }

    ShaderEffectSource {
        id: blowoutThousandSeven
        sourceItem: blowoutThousandSevenImg
    }

    ShaderEffect {
        id:thousandSevenFilter
        anchors.fill: parent

        property variant inputImageTexture: itemSource
        property variant inputImageTexture2: mapThousandSeven

        fragmentShader: "
            varying highp vec2 qt_TexCoord0;

            uniform sampler2D inputImageTexture;  //source
            uniform sampler2D inputImageTexture2; //map;

            void main() {
                vec3 texel = texture2D(inputImageTexture, qt_TexCoord0).rgb;
                texel = vec3(
                             texture2D(inputImageTexture2, vec2(texel.r, .16666)).r,
                             texture2D(inputImageTexture2, vec2(texel.g, .5)).g,
                             texture2D(inputImageTexture2, vec2(texel.b, .83333)).b);
                gl_FragColor = vec4(texel, 1.0);
            }"
    }
}
