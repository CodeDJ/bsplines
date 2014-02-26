#version 330

in vec2 tc;
uniform sampler2D tex;

layout ( location = 0 ) out vec4 fragmentColor;

void main(void)
{
    fragmentColor = texelFetch(tex, ivec2(tc.x, tc.y), 0);
}
