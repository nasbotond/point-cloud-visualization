#version 330 core

// per-fragment attributes coming from the pipeline
in vec3 vs_out_pos;
in vec3 vs_out_modelpos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

out vec4 fs_out_col;

// directional light source 
uniform vec3 light_dir = vec3(-1,-2,-0.5);

// light attributes: ambient, diffuse
uniform vec3 La = vec3(0.4, 0.4, 0.4);
uniform vec3 Ld = vec3(0.6, 0.6, 0.6);

uniform sampler2D texImage;

void main()
{
/*
	vec3 ambient = La;

	vec3 normal = normalize(vs_out_norm);
	vec3 to_light = normalize(-light_dir);
	
	float cosa = clamp(dot(normal, to_light), 0, 1);

	vec3 diffuse = cosa*Ld;
	
	fs_out_col = vec4(ambient + diffuse, 1) * texture(texImage, vs_out_tex);
*/
	fs_out_col = vec4(vs_out_modelpos, 1);
}
