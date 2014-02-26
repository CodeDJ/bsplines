#version 400
layout( vertices=%CONTROL_POINTS_COUNT% ) out;
uniform int stripsPerSegment = 30;
uniform int segmentsPerSpline = 4;
void main()
{
	// Pass along the vertex position unmodified
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	// may require to be swapped)
        gl_TessLevelOuter[0] = float(segmentsPerSpline);
        gl_TessLevelOuter[1] = float(stripsPerSegment);
}
