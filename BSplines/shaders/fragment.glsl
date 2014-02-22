#version 330
in vec3 SegmentColor;
uniform vec4 pointColor;
layout ( location = 0 ) out vec4 fragmentColor;
void main()
{
	fragmentColor = pointColor;
}
