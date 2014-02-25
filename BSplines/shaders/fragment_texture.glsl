#version 330

in vec2 tc;
uniform sampler2D tex;

layout ( location = 0 ) out vec4 fragmentColor;

void main(void)
{
    //fragmentColor = texture(tex, ivec2(texCoord.y, texCoord.x));
    fragmentColor = texelFetch(tex, ivec2(tc.x, tc.y), 0);

    //fragmentColor = texture(tex, ivec2(gl_FragCoord.x * 100, gl_FragCoord.y * 100));
    //vec4 color = texelFetch(tex, ivec2(1, 1), 0);
    //fragmentColor = vec4(1.0, 1.0, 1.0, 1.0);
}
