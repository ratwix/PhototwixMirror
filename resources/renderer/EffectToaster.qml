import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    property variant itemSource

    Rectangle {
        anchors.fill: parent
        color: "white"
    }

    Image {
        id:toasterMetalImg
        visible: false
        source: "../filtersResources/toasterMetal.png"
    }

    ShaderEffectSource {
        id: toasterMetal
        sourceItem: toasterMetalImg
    }

    Image {
        id:toasterSoftLightImg
        visible: false
        source: "../filtersResources/toasterSoftLight.png"
    }

    ShaderEffectSource {
        id: toasterSoftLight
        sourceItem: toasterSoftLightImg
    }

    Image {
        id:toasterCurvesImg
        visible: false
        source: "../filtersResources/toasterCurves.png"
    }

    ShaderEffectSource {
        id: toasterCurves
        sourceItem: toasterCurvesImg
    }

    Image {
        id:toasterOverlayMapWarmImg
        visible: false
        source: "../filtersResources/toasterOverlayMapWarm.png"
    }

    ShaderEffectSource {
        id: toasterOverlayMapWarm
        sourceItem: toasterOverlayMapWarmImg
    }

    Image {
        id:toasterColorShiftImg
        visible: false
        source: "../filtersResources/toasterColorShift.png"
    }

    ShaderEffectSource {
        id: toasterColorShift
        sourceItem: toasterColorShiftImg
    }

    ShaderEffect {
        id:toasterFilter
        anchors.fill: parent

        property variant inputImageTexture: itemSource
        property variant inputImageTexture2: toasterMetal
        property variant inputImageTexture3: toasterSoftLight
        property variant inputImageTexture4: toasterCurves
        property variant inputImageTexture5: toasterOverlayMapWarm
        property variant inputImageTexture6: toasterColorShift

        fragmentShader: "
                     varying highp vec2 qt_TexCoord0;

                     uniform sampler2D inputImageTexture;
                     uniform sampler2D inputImageTexture2; //toasterMetal
                     uniform sampler2D inputImageTexture3; //toasterSoftlight
                     uniform sampler2D inputImageTexture4; //toasterCurves
                     uniform sampler2D inputImageTexture5; //toasterOverlayMapWarm
                     uniform sampler2D inputImageTexture6; //toasterColorshift

                     void main()
                     {
                         lowp vec3 texel;
                         mediump vec2 lookup;
                         vec2 blue;
                         vec2 green;
                         vec2 red;
                         lowp vec4 tmpvar_1;
                         tmpvar_1 = texture2D (inputImageTexture, qt_TexCoord0);
                         texel = tmpvar_1.xyz;
                         lowp vec4 tmpvar_2;
                         tmpvar_2 = texture2D (inputImageTexture2, qt_TexCoord0);
                         lowp vec2 tmpvar_3;
                         tmpvar_3.x = tmpvar_2.x;
                         tmpvar_3.y = tmpvar_1.x;
                         texel.x = texture2D (inputImageTexture3, tmpvar_3).x;
                         lowp vec2 tmpvar_4;
                         tmpvar_4.x = tmpvar_2.y;
                         tmpvar_4.y = tmpvar_1.y;
                         texel.y = texture2D (inputImageTexture3, tmpvar_4).y;
                         lowp vec2 tmpvar_5;
                         tmpvar_5.x = tmpvar_2.z;
                         tmpvar_5.y = tmpvar_1.z;
                         texel.z = texture2D (inputImageTexture3, tmpvar_5).z;
                         red.x = texel.x;
                         red.y = 0.16666;
                         green.x = texel.y;
                         green.y = 0.5;
                         blue.x = texel.z;
                         blue.y = 0.833333;
                         texel.x = texture2D (inputImageTexture4, red).x;
                         texel.y = texture2D (inputImageTexture4, green).y;
                         texel.z = texture2D (inputImageTexture4, blue).z;
                         mediump vec2 tmpvar_6;
                         tmpvar_6 = ((2.0 * qt_TexCoord0) - 1.0);
                         mediump vec2 tmpvar_7;
                         tmpvar_7.x = dot (tmpvar_6, tmpvar_6);
                         tmpvar_7.y = texel.x;
                         lookup = tmpvar_7;
                         texel.x = texture2D (inputImageTexture5, tmpvar_7).x;
                         lookup.y = texel.y;
                         texel.y = texture2D (inputImageTexture5, lookup).y;
                         lookup.y = texel.z;
                         texel.z = texture2D (inputImageTexture5, lookup).z;
                         red.x = texel.x;
                         green.x = texel.y;
                         blue.x = texel.z;
                         texel.x = texture2D (inputImageTexture6, red).x;
                         texel.y = texture2D (inputImageTexture6, green).y;
                         texel.z = texture2D (inputImageTexture6, blue).z;
                         lowp vec4 tmpvar_8;
                         tmpvar_8.w = 1.0;
                         tmpvar_8.xyz = texel;
                         gl_FragColor = tmpvar_8;
                     }"
    }
}
