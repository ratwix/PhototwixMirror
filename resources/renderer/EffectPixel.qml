import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    property variant itemSource

    Rectangle {
        anchors.fill: parent
        color: "white"
    }

    ShaderEffect {
        id:amaroFilter
        anchors.fill: parent

        property variant inputImageTexture: itemSource
        property real  rt_w : parent.width;
        property real  rt_h : parent.height;
        property real  pixel_w : 10; // 15.0
        property real  pixel_h : 5; // 10.0

        fragmentShader: "
                        varying highp vec2 qt_TexCoord0;

                        uniform sampler2D inputImageTexture;

                        uniform float rt_w; // GeeXLab built-in
                        uniform float rt_h; // GeeXLab built-in
                        uniform float pixel_w; // 15.0
                        uniform float pixel_h; // 10.0

                        void main()
                        {
                              vec2 uv = qt_TexCoord0.xy;

                              vec3 tc = vec3(1.0, 0.0, 0.0);

                              float dx = pixel_w*(1./rt_w);
                              float dy = pixel_h*(1./rt_h);
                              vec2 coord = vec2(dx*floor(uv.x/dx),
                                                  dy*floor(uv.y/dy));
                              tc = texture2D(inputImageTexture, coord).rgb;

                              gl_FragColor = vec4(tc, 1.0);
                        }"
    }
}
