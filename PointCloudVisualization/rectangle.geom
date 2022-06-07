#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 10) out;

in VS_OUT {
    vec3 normal;
	int l;
	int w;
    vec3 color;
} gs_in[];

out vec3 fColor;

uniform mat4 projection;

void main()
{
    fColor = gs_in[0].color;

    vec3 u = vec3(1, -1, 0);
    vec3 v2 = cross(gs_in[0].normal, u);
    vec3 v3 = normalize(cross(gs_in[0].normal, v2));
    v2 = normalize(v2);
    
    /*
    gl_Position = projection * (gl_in[0].gl_Position - vec4(v2, 0.0)) + projection * (gl_in[0].gl_Position + vec4(v3, 0.0) * 1 * 0.2);    // 1:bottom-left
    EmitVertex();
    gl_Position = projection * (gl_in[0].gl_Position - vec4(v2, 0.0) - vec4(v3, 0.0) * 1 * 0.2);    // 2:bottom-right
    EmitVertex();
    gl_Position = projection * (gl_in[0].gl_Position + vec4(v2, 0.0) + vec4(v3, 0.0) * 1 * 0.2);    // 3:top-left
    EmitVertex();
    gl_Position = projection * (gl_in[0].gl_Position + vec4(v2, 0.0) - vec4(v3, 0.0) * 1 * 0.2);    // 4:top-right
    EmitVertex();

    gl_Position = projection * (gl_in[0].gl_Position + vec4(v2, 0.0) + vec4(v3, 0.0) * 1 * 0.2);    // 1:bottom-left
    EmitVertex();
    gl_Position = projection * (gl_in[0].gl_Position + vec4(v2, 0.0) - vec4(v3, 0.0) * 1 * 0.2);    // 2:bottom-right
    EmitVertex();
    gl_Position = projection * (gl_in[0].gl_Position - vec4(v2, 0.0) + vec4(v3, 0.0) * 1 * 0.2);    // 3:top-left
    EmitVertex();
    gl_Position = projection * (gl_in[0].gl_Position - vec4(v2, 0.0) - vec4(v3, 0.0) * 1 * 0.2);    // 4:top-right
    EmitVertex();
    */

    
    gl_Position = (projection * (gl_in[0].gl_Position - vec4(v2, 0.0) * gs_in[0].l * 0.1)) + (projection * (gl_in[0].gl_Position + vec4(v3, 0.0) * gs_in[0].w * 0.1));    // 1:bottom-left
    EmitVertex();
    gl_Position = (projection * (gl_in[0].gl_Position - vec4(v2, 0.0) * gs_in[0].l * 0.1)) + (projection * (gl_in[0].gl_Position - vec4(v3, 0.0) * gs_in[0].w * 0.1));    // 2:bottom-right
    EmitVertex();
    gl_Position = (projection * (gl_in[0].gl_Position + vec4(v2, 0.0) * gs_in[0].l * 0.1)) + (projection * (gl_in[0].gl_Position + vec4(v3, 0.0) * gs_in[0].w * 0.1));    // 3:top-left
    EmitVertex();
    gl_Position = (projection * (gl_in[0].gl_Position + vec4(v2, 0.0) * gs_in[0].l * 0.1)) + (projection * (gl_in[0].gl_Position - vec4(v3, 0.0) * gs_in[0].w * 0.1));    // 4:top-right
    EmitVertex();

    gl_Position = (projection * (gl_in[0].gl_Position + vec4(v2, 0.0) * gs_in[0].l * 0.1)) + (projection * (gl_in[0].gl_Position + vec4(v3, 0.0) * gs_in[0].w * 0.1));    // 1:bottom-left
    EmitVertex();
    gl_Position = (projection * (gl_in[0].gl_Position + vec4(v2, 0.0) * gs_in[0].l * 0.1)) + (projection * (gl_in[0].gl_Position - vec4(v3, 0.0) * gs_in[0].w * 0.1));    // 2:bottom-right
    EmitVertex();
    gl_Position = (projection * (gl_in[0].gl_Position - vec4(v2, 0.0) * gs_in[0].l * 0.1)) + (projection * (gl_in[0].gl_Position + vec4(v3, 0.0) * gs_in[0].w * 0.1));    // 3:top-left
    EmitVertex();
    gl_Position = (projection * (gl_in[0].gl_Position - vec4(v2, 0.0) * gs_in[0].l * 0.1)) + (projection * (gl_in[0].gl_Position - vec4(v3, 0.0) * gs_in[0].w * 0.1));    // 4:top-right
    EmitVertex();

    EndPrimitive();
}  