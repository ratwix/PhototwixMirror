import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    property variant itemSource

    Rectangle {
        anchors.fill: parent
        color: "white"
    }

    Image {
        id:nashvilleMapImg
        visible: false
        source: "../filtersResources/nashvilleMap.png"
    }

    ShaderEffectSource {
        id: nashvilleMap
        sourceItem: nashvilleMapImg
    }

    ShaderEffect {
        id:nashvilleFilter
        anchors.fill: parent

        property variant inputImageTexture: itemSource
        property variant inputImageTexture2: nashvilleMap

        fragmentShader: "
                         varying highp vec2 qt_TexCoord0;

                         uniform sampler2D inputImageTexture;
                         uniform sampler2D inputImageTexture2;

                         void main()
                         {
                             vec3 texel = texture2D(inputImageTexture, qt_TexCoord0).rgb;
                             texel = vec3(
                                          texture2D(inputImageTexture2, vec2(texel.r, .16666)).r,
                                          texture2D(inputImageTexture2, vec2(texel.g, .5)).g,
                                          texture2D(inputImageTexture2, vec2(texel.b, .83333)).b);
                             gl_FragColor = vec4(texel, 1.0);
                         }"
    }
}
