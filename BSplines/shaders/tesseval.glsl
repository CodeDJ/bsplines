#version 400

layout( isolines ) in;
uniform int segmentsPerSpline = 4;
out vec3 tesSegmentColor;

vec3 quadratic_bezier(vec3 p0, vec3 p1, vec3 p2, float u)
{
	vec3 p01 = mix(p0, p1, u);
	vec3 p12 = mix(p1, p2, u);
	vec3 p = mix(p01, p12, u);
	return p;
}

vec3 cubic_bezier(vec3 p0, vec3 p1, vec3 p2, vec3 p3, float u)
{
	vec3 p01 = mix(p0, p1, u);
	vec3 p12 = mix(p1, p2, u);
	vec3 p23 = mix(p2, p3, u);
	return quadratic_bezier(p01, p12, p23, u);
}

vec3 cubic_bspline(vec3 p[4], float u)
{
	if (u <= 0.0) return p[0];
	if (u >= 1.0) return p[3];
	return cubic_bezier(p[0], p[1], p[2], p[3], u);
}

vec3 evaluateBezierPosition( vec3 v[4], float t )
{
	float OneMinusT = 1.0 - t;
	float b0 = OneMinusT*OneMinusT*OneMinusT;
	float b1 = 3.0*t*OneMinusT*OneMinusT;
	float b2 = 3.0*t*t*OneMinusT;
	float b3 = t*t*t;
	return b0*v[0] + b1*v[1] + b2*v[2] + b3*v[3];
}

void main()
{
	// The tessellation u coordinate
	float u = gl_TessCoord.x;
        int step = int(floor(100.0/segmentsPerSpline));
	int v = int(floor(gl_TessCoord.y*100.0))/step;
	//// The patch vertices (control points)
	vec3 p[4]; 
	for (int i=0; i<4; ++i) p[i] = gl_in[v*3+i].gl_Position.xyz;
	// Cubic Bezier interpolation
	vec3 pf = cubic_bspline(p, u);
	//vec3 pf = evaluateBezierPosition(p, u);
	gl_Position = vec4(pf, 1.0); 
	if (v==0) tesSegmentColor = vec3(1.0, 0.0, 0.0);
	else if (v==1) tesSegmentColor = vec3(0.0, 1.0, 0.0);
	else if (v==2) tesSegmentColor = vec3(0.0, 0.0, 1.0);
	else if (v==3) tesSegmentColor = vec3(1.0, 1.0, 0.0);
	else tesSegmentColor = vec3(1.0, 1.0, 1.0);
}


