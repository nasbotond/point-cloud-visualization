#version 330 core

// per-fragment attributes coming from the pipeline
in vec3 vs_out_pos;
in vec3 vs_out_modelpos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;
in vec3 vs_out_col;
in float vs_out_texOrCol;

out vec4 fs_out_col;

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
	if(vs_out_texOrCol == 0.0)
	{
		fs_out_col = vec4(vs_out_col, 1);
	}
	else
	{
		fs_out_col = texture(texImage, vs_out_tex);
	}
}