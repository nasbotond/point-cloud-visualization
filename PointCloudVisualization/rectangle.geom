#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
    vec3 normal;
	int l;
	int w;
    vec3 color;
    vec2 tex;
} gs_in[];

out vec3 fColor;
out vec2 gs_out_tex;

uniform mat4 MVP;

void main()
{
    fColor = gs_in[0].color;
    // gs_out_tex = gs_in[0].tex;

    vec3 u = vec3(1, 1, 1);
    vec3 v2 = cross(gs_in[0].normal, u);
    vec3 v3 = normalize(cross(gs_in[0].normal, v2));
    v2 = normalize(v2);

    const vec2 coordinates [] = vec2 [] (vec2 (0.0f, 0.0f), // bottom left
                                       vec2 (1.0f, 0.0f), // bottom right
                                       vec2 (1.0f, 1.0f), // top right
                                       vec2 (0.0f, 1.0f)); // top left

    
    gl_Position = (MVP * (gl_in[0].gl_Position - vec4(v2, 0.0) * gs_in[0].l * 0.01)) + (MVP * (gl_in[0].gl_Position + vec4(v3, 0.0) * gs_in[0].w * 0.01));    // 1:bottom-left
    gs_out_tex = coordinates[1];
    EmitVertex();
    gl_Position = (MVP * (gl_in[0].gl_Position - vec4(v2, 0.0) * gs_in[0].l * 0.01)) + (MVP * (gl_in[0].gl_Position - vec4(v3, 0.0) * gs_in[0].w * 0.01));    // 2:bottom-right
    gs_out_tex = coordinates[0];
    EmitVertex();
    gl_Position = (MVP * (gl_in[0].gl_Position + vec4(v2, 0.0) * gs_in[0].l * 0.01)) + (MVP * (gl_in[0].gl_Position + vec4(v3, 0.0) * gs_in[0].w * 0.01));    // 3:top-left
    gs_out_tex = coordinates[2];
    EmitVertex();
    gl_Position = (MVP * (gl_in[0].gl_Position + vec4(v2, 0.0) * gs_in[0].l * 0.01)) + (MVP * (gl_in[0].gl_Position - vec4(v3, 0.0) * gs_in[0].w * 0.01));    // 4:top-right
    gs_out_tex = coordinates[3];
    EmitVertex();

    EndPrimitive();
}  