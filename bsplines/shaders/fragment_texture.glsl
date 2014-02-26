#version 330

in vec2 tc;
uniform sampler2D tex;
uniform float finalAlpha = -0.1;

layout ( location = 0 ) out vec4 fragmentColor;

void main(void)
{
    fragmentColor = texelFetch(tex, ivec2(tc.x, tc.y), 0);
    if (finalAlpha >= 0.0)
    {
        fragmentColor.a = finalAlpha;
    }
}
