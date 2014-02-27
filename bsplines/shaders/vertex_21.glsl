#version 120

uniform vec2 controlPoints[%CONTROL_POINTS_COUNT%];
uniform int passThrough = 1;

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
    if (passThrough == 0)
    {
        vec3 p[4];
        int y = int(floor(gl_Vertex.y));
        p[0] = vec3(controlPoints[3*y + 0], 0.0);
        p[1] = vec3(controlPoints[3*y + 1], 0.0);
        p[2] = vec3(controlPoints[3*y + 2], 0.0);
        p[3] = vec3(controlPoints[3*y + 3], 0.0);
        float t =  gl_Vertex.x;
        gl_Position = vec4(evaluateBezierPosition(p, t), 1.0);
    }
    else
    {
        gl_Position = gl_Vertex;
    }
}
