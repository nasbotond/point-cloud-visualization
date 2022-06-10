#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 10) out;

in VS_OUT {
    vec3 normal;
    int magnitude;
} gs_in[];

uniform mat4 MVP;

void main()
{
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = (gl_in[0].gl_Position + MVP * vec4(gs_in[0].normal, 0.0) * gs_in[0].magnitude * 0.01);
    EmitVertex();
    EndPrimitive();
}  