#version 150
layout (lines) in;
layout (triangle_strip) out;
uniform vec2 ControlPoints[13];
uniform float lineWidthAlphaX = 0.003;
uniform float lineWidthAlphaY = 0.003;
layout(max_vertices = 1024) out;

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

void process(vec3 p0, vec3 p1, bool all) 
{
	vec3 v = p1 - p0;
	vec4 n1 = vec4(normalize(vec3(-v.y, v.x, 0.0)), 0.0);
	vec4 n2 = vec4(normalize(vec3(v.y, -v.x, 0.0)), 0.0);
	n1.x = n1.x * lineWidthAlphaX / 2; 
	n1.y = n1.y * lineWidthAlphaY / 2; 
	n2.x = n2.x * lineWidthAlphaX / 2; 
	n2.y = n2.y * lineWidthAlphaY / 2; 
	gl_Position = vec4(p1, 1.0) + n2;//lineWidthAlpha*n2;
	EmitVertex();
	gl_Position = vec4(p1, 1.0) + n1;//lineWidthAlpha*n1;
	EmitVertex();
}

void process_all(vec3 p0, vec3 p1, bool all)
{
	vec3 v = p1 - p0;
	vec4 n1 = vec4(normalize(vec3(-v.y, v.x, 0.0)), 0.0);
	vec4 n2 = vec4(normalize(vec3(v.y, -v.x, 0.0)), 0.0);
	n1.x = n1.x * lineWidthAlphaX / 2; 
	n1.y = n1.y * lineWidthAlphaY / 2; 
	n2.x = n2.x * lineWidthAlphaX / 2; 
	n2.y = n2.y * lineWidthAlphaY / 2; 
	gl_Position = vec4(p0, 1.0) + n2;//lineWidthAlphaX*n2;
	EmitVertex();
	gl_Position = vec4(p0, 1.0) + n1;//lineWidthAlpha*n1;
	EmitVertex();
	gl_Position = vec4(p1, 1.0) + n2;//lineWidthAlpha*n2;
	EmitVertex();
	gl_Position = vec4(p1, 1.0) + n1;//lineWidthAlpha*n1;
	EmitVertex();
}


void main()
{
	vec3 p[4];
	vec3 prev, crt;
	float step = 1.0/30.0;
	for (int v=0; v<4; ++v)
	{
   		p[0] = vec3(ControlPoints[3*v], 0.0);
   		p[1] = vec3(ControlPoints[3*v + 1], 0.0);
   		p[2] = vec3(ControlPoints[3*v + 2], 0.0);
   		p[3] = vec3(ControlPoints[3*v + 3], 0.0);
   		float u = 0.0;
   		//prev = cubic_bspline(p, u);
   		prev = p[0];
   		u += step;
   		//crt = cubic_bspline(p, u);
   		crt = evaluateBezierPosition(p, u);
   		process_all(prev, crt, true);
   		prev = crt;
   		for(int j=2; j<30; ++j)
		{ 
       		u += step;
       		//crt = cubic_bspline(p, u);
       		crt = evaluateBezierPosition(p, u);
       		process(prev, crt, false);
       		prev = crt;
		}  // End for(j)
		crt = p[3];
		process(prev, crt, false);
	} // End for(v)
	EndPrimitive();
}
