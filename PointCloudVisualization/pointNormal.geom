#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 10) out;

// in vec3 vs_out_pos;
// in vec3 vs_out_modelpos;
// in vec3 vs_out_norm;

in VS_OUT {
    vec3 normal;
    int magnitude;
} gs_in[];

// const float MAGNITUDE = 1.0;

uniform mat4 projection;

void main()
{
    gl_Position = projection * gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = projection * (gl_in[0].gl_Position + vec4(gs_in[0].normal, 0.0) * gs_in[0].magnitude * 0.5);
    EmitVertex();
    EndPrimitive();
}  