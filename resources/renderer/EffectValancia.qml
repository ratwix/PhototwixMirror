import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    property variant itemSource

    Rectangle {
        anchors.fill: parent
        color: "white"
    }

    Image {
        id:valenciaMapImg
        visible: false
        source: "../filtersResources/valenciaMap.png"
    }

    ShaderEffectSource {
        id: valenciaMap
        sourceItem: valenciaMapImg
    }

    Image {
        id:valenciaGradientMapImg
        visible: false
        source: "../filtersResources/valenciaGradientMap.png"
    }

    ShaderEffectSource {
        id: valenciaGradientMap
        sourceItem: valenciaGradientMapImg
    }

    ShaderEffect {
        id:valanciaFilter
        anchors.fill: parent

        property variant inputImageTexture: itemSource
        property variant inputImageTexture2: valenciaMap
        property variant inputImageTexture3: valenciaGradientMap

        fragmentShader: "
                    varying highp vec2 qt_TexCoord0;

                    uniform sampler2D inputImageTexture;
                    uniform sampler2D inputImageTexture2; //map
                    uniform sampler2D inputImageTexture3; //gradMap

                    mat3 saturateMatrix = mat3(
                                                1.1402,
                                                -0.0598,
                                                -0.061,
                                                -0.1174,
                                                1.0826,
                                                -0.1186,
                                                -0.0228,
                                                -0.0228,
                                                1.1772);

                     vec3 lumaCoeffs = vec3(.3, .59, .11);

                     void main()
                     {
                         vec3 texel = texture2D(inputImageTexture, qt_TexCoord0).rgb;

                         texel = vec3(
                                      texture2D(inputImageTexture2, vec2(texel.r, .1666666)).r,
                                      texture2D(inputImageTexture2, vec2(texel.g, .5)).g,
                                      texture2D(inputImageTexture2, vec2(texel.b, .8333333)).b
                                      );

                         texel = saturateMatrix * texel;
                         float luma = dot(lumaCoeffs, texel);
                         texel = vec3(
                                      texture2D(inputImageTexture3, vec2(luma, texel.r)).r,
                                      texture2D(inputImageTexture3, vec2(luma, texel.g)).g,
                                      texture2D(inputImageTexture3, vec2(luma, texel.b)).b);

                         gl_FragColor = vec4(texel, 1.0);
                     }"
    }
}
