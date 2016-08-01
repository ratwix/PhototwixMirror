import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    property variant itemSource

    Rectangle {
        anchors.fill: parent
        color: "white"
    }

    Image {
            id:edgeBurnImg
            visible: false
            source: "../filtersResources/edgeBurn.png"
        }

        ShaderEffectSource {
            id: edgeBurn
            sourceItem: edgeBurnImg
        }

        Image {
            id:hefeMapImg
            visible: false
            source: "../filtersResources/hefeMap.png"
        }

        ShaderEffectSource {
            id: hefeMap
            sourceItem: hefeMapImg
        }

        Image {
            id:hefeGradientMapImg
            visible: false
            source: "../filtersResources/hefeGradientMap.png"
        }

        ShaderEffectSource {
            id: hefeGradientMap
            sourceItem: hefeGradientMapImg
        }


        Image {
            id:hefeSoftLightImg
            visible: false
            source: "../filtersResources/hefeSoftLight.png"
        }

        ShaderEffectSource {
            id: hefeSoftLight
            sourceItem: hefeSoftLightImg
        }

        Image {
            id:hefeMetalImg
            visible: false
            source: "../filtersResources/hefeMetal.png"
        }

        ShaderEffectSource {
            id: hefeMetal
            sourceItem: hefeMetalImg
        }

    ShaderEffect {
        id:hefeFilter
        anchors.fill: parent

        property variant inputImageTexture: itemSource
        property variant inputImageTexture2: edgeBurn
        property variant inputImageTexture3: hefeMap
        property variant inputImageTexture4: hefeGradientMap
        property variant inputImageTexture5: hefeSoftLightImg
        property variant inputImageTexture6: hefeSoftLight

        fragmentShader: "
         varying highp vec2 qt_TexCoord0;

         uniform sampler2D inputImageTexture;
         uniform sampler2D inputImageTexture2;  //edgeBurn
         uniform sampler2D inputImageTexture3;  //hefeMap
         uniform sampler2D inputImageTexture4;  //hefeGradientMap
         uniform sampler2D inputImageTexture5;  //hefeSoftLight
         uniform sampler2D inputImageTexture6;  //hefeMetal

         void main()
        {
            vec3 texel = texture2D(inputImageTexture, qt_TexCoord0).rgb;
            vec3 edge = texture2D(inputImageTexture2, qt_TexCoord0).rgb;
            texel = texel * edge;

            texel = vec3(
                         texture2D(inputImageTexture3, vec2(texel.r, .16666)).r,
                         texture2D(inputImageTexture3, vec2(texel.g, .5)).g,
                         texture2D(inputImageTexture3, vec2(texel.b, .83333)).b);

            vec3 luma = vec3(.30, .59, .11);
            vec3 gradSample = texture2D(inputImageTexture4, vec2(dot(luma, texel), .5)).rgb;
            vec3 final = vec3(
                              texture2D(inputImageTexture5, vec2(gradSample.r, texel.r)).r,
                              texture2D(inputImageTexture5, vec2(gradSample.g, texel.g)).g,
                              texture2D(inputImageTexture5, vec2(gradSample.b, texel.b)).b
                              );

            vec3 metal = texture2D(inputImageTexture6, qt_TexCoord0).rgb;
            vec3 metaled = vec3(
                                texture2D(inputImageTexture5, vec2(metal.r, texel.r)).r,
                                texture2D(inputImageTexture5, vec2(metal.g, texel.g)).g,
                                texture2D(inputImageTexture5, vec2(metal.b, texel.b)).b
                                );

            gl_FragColor = vec4(metaled, 1.0);
        }"
    }
}
