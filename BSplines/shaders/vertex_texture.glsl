#version 330
layout (location = 0 ) in vec4 VertexPosition;

out vec2 tc;

void main(void)
{
    gl_Position = vec4(VertexPosition.xy, 0.0, 1.0);
    tc = VertexPosition.zw;
}
