#version 120

varying vec2 tc;
uniform sampler2D tex;
uniform float finalAlpha = -0.1;

void main(void)
{
    gl_FragColor = texture2D(tex, vec2(tc.x, tc.y), 0);
    if (finalAlpha >= 0.0)
    {
        gl_FragColor.a = finalAlpha;
    }
}
