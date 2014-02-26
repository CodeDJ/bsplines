#version 330
layout (location = 0 ) in vec4 VertexPosition;

uniform float alphaX = 0.001;
uniform float alphaY = 0.001;

out vec2 tc;

void main(void)
{
    gl_Position = vec4(VertexPosition.x*alphaX, VertexPosition.y*alphaY, 0.0, 1.0);
    tc = VertexPosition.zw;
}
