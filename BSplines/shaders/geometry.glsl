#version 400
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;
in vec3 tesSegmentColor[];
out vec3 SegmentColor;
uniform float lineWidthAlphaX = 0.003;
uniform float lineWidthAlphaY = 0.003;
void main()
{
	SegmentColor = tesSegmentColor[0];
	float pixels = 0.003; 
	vec3 v = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec4 n1 = vec4(normalize(vec3(-v.y, v.x, 0.0)), 0.0);
	vec4 n2 = vec4(normalize(vec3(v.y, -v.x, 0.0)), 0.0);
	n1.x = n1.x * lineWidthAlphaX / 2; 
	n1.y = n1.y * lineWidthAlphaY / 2; 
	n2.x = n2.x * lineWidthAlphaX / 2; 
	n2.y = n2.y * lineWidthAlphaY / 2; 
	gl_Position = gl_in[0].gl_Position + n2;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + n1;
	EmitVertex();
	gl_Position= gl_in[1].gl_Position + n2;
	EmitVertex();
	gl_Position = gl_in[1].gl_Position + n1;
	EmitVertex();
}
