import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    property variant itemSource

    Rectangle {
        anchors.fill: parent
        color: "white"
    }

    Image {
        id:blackboard1024Img
        visible: false
        source: "../filtersResources/blackboard1024.png"
    }

    ShaderEffectSource {
        id: blackboard1024
        sourceItem: blackboard1024Img
    }

    Image {
        id:overlayMapImg
        visible: false
        source: "../filtersResources/overlayMap.png"
    }

    ShaderEffectSource {
        id: overlayMap
        sourceItem: overlayMapImg
    }

    Image {
        id:riseMapImg
        visible: false
        source: "../filtersResources/riseMap.png"
    }

    ShaderEffectSource {
        id: riseMap
        sourceItem: riseMapImg
    }

    ShaderEffect {
        id:riseFilter
        anchors.fill: parent

        property variant inputImageTexture: itemSource
        property variant inputImageTexture2: blackboard1024
        property variant inputImageTexture3: overlayMap
        property variant inputImageTexture4: riseMap

        fragmentShader: "
                         varying highp vec2 qt_TexCoord0;

                         uniform sampler2D inputImageTexture;
                         uniform sampler2D inputImageTexture2;
                         uniform sampler2D inputImageTexture3;
                         uniform sampler2D inputImageTexture4;

                         void main()
                         {
                             vec4 texel = texture2D(inputImageTexture, qt_TexCoord0);
                             vec3 bbTexel = texture2D(inputImageTexture2, qt_TexCoord0).rgb;

                             texel.r = texture2D(inputImageTexture3, vec2(bbTexel.r, texel.r)).r;
                             texel.g = texture2D(inputImageTexture3, vec2(bbTexel.g, texel.g)).g;
                             texel.b = texture2D(inputImageTexture3, vec2(bbTexel.b, texel.b)).b;

                             vec4 mapped;
                             mapped.r = texture2D(inputImageTexture4, vec2(texel.r, .16666)).r;
                             mapped.g = texture2D(inputImageTexture4, vec2(texel.g, .5)).g;
                             mapped.b = texture2D(inputImageTexture4, vec2(texel.b, .83333)).b;
                             mapped.a = 1.0;

                             gl_FragColor = mapped;
                         }"
    }
}
