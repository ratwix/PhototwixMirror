import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    property variant itemSource

    Rectangle {
        anchors.fill: parent
        color: "white"
    }

    //use vignetteMap

    Image {
        id:vignetteMapImg
        visible: false
        source: "../filtersResources/vignetteMap.png"
    }

    ShaderEffectSource {
        id: vignetteMap
        sourceItem: vignetteMapImg
    }

        Image {
            id:sutroMetalImg
            visible: false
            source: "../filtersResources/sutroMetal.png"
        }

        ShaderEffectSource {
            id: sutroMetal
            sourceItem: sutroMetalImg
        }


        Image {
            id:softLightImg
            visible: false
            source: "../filtersResources/softLight.png"
        }

        ShaderEffectSource {
            id: softLight
            sourceItem: softLightImg
        }

        Image {
            id:sutroEdgeBurnImg
            visible: false
            source: "../filtersResources/sutroEdgeBurn.png"
        }

        ShaderEffectSource {
            id: sutroEdgeBurn
            sourceItem: sutroEdgeBurnImg
        }

        Image {
            id:sutroCurvesImg
            visible: false
            source: "../filtersResources/sutroCurves.png"
        }

        ShaderEffectSource {
            id: sutroCurves
            sourceItem: sutroCurvesImg
        }

    ShaderEffect {
        id:amaroFilter
        anchors.fill: parent

        property variant inputImageTexture: itemSource
        property variant inputImageTexture2: vignetteMap
        property variant inputImageTexture3: sutroMetal
        property variant inputImageTexture4: softLight
        property variant inputImageTexture5: sutroEdgeBurn
        property variant inputImageTexture6: sutroCurves

        fragmentShader: "
                     varying highp vec2 qt_TexCoord0;

                     uniform sampler2D inputImageTexture;
                     uniform sampler2D inputImageTexture2; //sutroMap;
                     uniform sampler2D inputImageTexture3; //sutroMetal;
                     uniform sampler2D inputImageTexture4; //softLight
                     uniform sampler2D inputImageTexture5; //sutroEdgeburn
                     uniform sampler2D inputImageTexture6; //sutroCurves

                     void main()
                     {
                         vec3 texel = texture2D(inputImageTexture, qt_TexCoord0).rgb;

                         vec2 tc = (2.0 * qt_TexCoord0) - 1.0;
                         float d = dot(tc, tc);
                         vec2 lookup = vec2(d, texel.r);
                         texel.r = texture2D(inputImageTexture2, lookup).r;
                         lookup.y = texel.g;
                         texel.g = texture2D(inputImageTexture2, lookup).g;
                         lookup.y = texel.b;
                         texel.b	= texture2D(inputImageTexture2, lookup).b;

                         vec3 rgbPrime = vec3(0.1019, 0.0, 0.0);
                         float m = dot(vec3(.3, .59, .11), texel.rgb) - 0.03058;
                         texel = mix(texel, rgbPrime + m, 0.32);

                         vec3 metal = texture2D(inputImageTexture3, qt_TexCoord0).rgb;
                         texel.r = texture2D(inputImageTexture4, vec2(metal.r, texel.r)).r;
                         texel.g = texture2D(inputImageTexture4, vec2(metal.g, texel.g)).g;
                         texel.b = texture2D(inputImageTexture4, vec2(metal.b, texel.b)).b;

                         texel = texel * texture2D(inputImageTexture5, qt_TexCoord0).rgb;

                         texel.r = texture2D(inputImageTexture6, vec2(texel.r, .16666)).r;
                         texel.g = texture2D(inputImageTexture6, vec2(texel.g, .5)).g;
                         texel.b = texture2D(inputImageTexture6, vec2(texel.b, .83333)).b;


                         gl_FragColor = vec4(texel, 1.0);
                     }"
    }
}
