#version 330
layout (location = 0 ) in vec2 VertexPosition;
out vec3 SegmentColor;
void main()
{
	gl_Position = vec4(VertexPosition, 0.0, 1.0);
	SegmentColor = vec3(1.0, 1.0, 1.0);
}
