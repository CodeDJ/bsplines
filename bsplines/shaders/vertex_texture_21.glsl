#version 120

uniform float alphaX = 0.001;
uniform float alphaY = 0.001;

varying vec2 tc;

void main(void)
{
    gl_Position = vec4(gl_Vertex.x*alphaX, gl_Vertex.y*alphaY, 0.0, 1.0);
    tc = gl_Vertex.zw;
}
