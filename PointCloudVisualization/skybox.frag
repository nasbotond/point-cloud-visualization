#version 330 core

// variables coming from the pipeline
in vec3 vs_out_pos;

out vec4 fs_out_col;

uniform samplerCube skyboxTexture;

void main()
{
	/*
	//fs_out_col = vec4( normalize(vs_out_pos), 1);
	fs_out_col = texture( skyboxTexture, (vs_out_pos) );
	
	// procedural 1:
	fs_out_col = vec4( vs_out_pos, 1);
	*/

	// procedural 2:

	vec3 dir = normalize(vs_out_pos);

	const vec3 asd = normalize(vec3(1,1,1));

	float mult = pow(clamp(dot(dir, asd),0,1),160.0);

	const vec4 groundColor = vec4(0.5, 0.2, 0.2, 1);
	const vec4 skyColor = vec4(0.2, 0.3, 0.7, 1);

	vec3 suncol = vec3(259,232,86)/255.0;

	fs_out_col = mix( groundColor, skyColor, (1 + dir.y)/2) + 1.2*mult*vec4(suncol,0);
}