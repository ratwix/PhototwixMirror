import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    property variant itemSource

    ShaderEffect {
        anchors.fill: parent
        property variant source: itemSource
        property real amount: 1.0
        fragmentShader: "
            uniform sampler2D source;
            uniform float amount;
            uniform lowp float qt_Opacity;
            varying vec2 qt_TexCoord0;

            void main()
            {
                vec2 uv = qt_TexCoord0.xy;
                vec4 color = texture2D(source, uv);
                float r = color.r;
                float g = color.g;
                float b = color.b;

                color.r = min(1.0, (r * (1.0 - (0.607 * amount))) + (g * (0.769 * amount)) + (b * (0.189 * amount)));
                color.g = min(1.0, (r * 0.349 * amount) + (g * (1.0 - (0.314 * amount))) + (b * 0.168 * amount));
                color.b = min(1.0, (r * 0.272 * amount) + (g * 0.534 * amount) + (b * (1.0 - (0.869 * amount))));

                gl_FragColor = color;

            }
        "
    }
}
