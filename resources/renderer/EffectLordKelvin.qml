import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    property variant itemSource

    Rectangle {
        anchors.fill: parent
        color: "white"
    }

    Image {
        id:kelvinMapImg
        visible: false
        source: "../filtersResources/kelvinMap.png"
    }

    ShaderEffectSource {
        id: kelvinMap
        sourceItem: kelvinMapImg
    }

    ShaderEffect {
        id:lordKelvinFilter
        anchors.fill: parent

        property variant inputImageTexture: itemSource
        property variant inputImageTexture2: kelvinMap

        fragmentShader: "
                         varying highp vec2 qt_TexCoord0;

                         uniform sampler2D inputImageTexture;
                         uniform sampler2D inputImageTexture2;

                         void main()
                         {
                            vec3 texel = texture2D(inputImageTexture, qt_TexCoord0).rgb;

                            vec2 lookup;
                            lookup.y = .5;

                            lookup.x = texel.r;
                            texel.r = texture2D(inputImageTexture2, lookup).r;

                            lookup.x = texel.g;
                            texel.g = texture2D(inputImageTexture2, lookup).g;

                            lookup.x = texel.b;
                            texel.b = texture2D(inputImageTexture2, lookup).b;

                            gl_FragColor = vec4(texel, 1.0);
                         }"
    }
}
