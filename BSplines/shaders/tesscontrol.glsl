#version 150
layout( vertices=13 ) out;
uniform int numSegments = 30;
uniform int numStrips = 3;
void main()
{
	// Pass along the vertex position unmodified
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	// may require to be swapped)
	gl_TessLevelOuter[0] = float(numStrips);
	gl_TessLevelOuter[1] = float(numSegments);
}
