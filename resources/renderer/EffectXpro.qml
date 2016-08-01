import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    property variant itemSource

    Rectangle {
        anchors.fill: parent
        color: "white"
    }

    Image {
        id:xproMapImg
        visible: false
        source: "../filtersResources/xproMap.png"
    }

    ShaderEffectSource {
        id: xproMap
        sourceItem: xproMapImg
    }

    Image {
        id:vignetteMapImg
        visible: false
        source: "../filtersResources/vignetteMap.png"
    }

    ShaderEffectSource {
        id: vignetteMap
        sourceItem: vignetteMapImg
    }

    ShaderEffect {
        id:xproFilter
        anchors.fill: parent

        property variant inputImageTexture: itemSource
        property variant inputImageTexture2: xproMap
        property variant inputImageTexture3: vignetteMap

        fragmentShader: "
            varying highp vec2 qt_TexCoord0;

            uniform sampler2D inputImageTexture;
            uniform sampler2D inputImageTexture2; //map
            uniform sampler2D inputImageTexture3; //vigMap

            void main()
            {
                 vec3 texel = texture2D(inputImageTexture, qt_TexCoord0).rgb;

                 vec2 tc = (2.0 * qt_TexCoord0) - 1.0;
                 float d = dot(tc, tc);
                 vec2 lookup = vec2(d, texel.r);
                 texel.r = texture2D(inputImageTexture3, lookup).r;
                 lookup.y = texel.g;
                 texel.g = texture2D(inputImageTexture3, lookup).g;
                 lookup.y = texel.b;
                 texel.b	= texture2D(inputImageTexture3, lookup).b;

                 vec2 red = vec2(texel.r, 0.16666);
                 vec2 green = vec2(texel.g, 0.5);
                 vec2 blue = vec2(texel.b, .83333);
                 texel.r = texture2D(inputImageTexture2, red).r;
                 texel.g = texture2D(inputImageTexture2, green).g;
                 texel.b = texture2D(inputImageTexture2, blue).b;

                 gl_FragColor = vec4(texel, 1.0);
            }"
    }
}
