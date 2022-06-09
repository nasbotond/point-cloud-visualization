#version 330 core

// variables coming from the pipeline
in vec3 vs_out_pos;

out vec4 fs_out_col;

uniform samplerCube skyboxTexture;
uniform int skyboxBackground;

void main()
{	
	if(skyboxBackground == 0)
	{
		
		vec3 dir = normalize(vs_out_pos);

		const vec4 groundColor = vec4(0, 0, 0, 1);
		const vec4 skyColor = vec4(0.2, 0.25, 0.5, 1);

		fs_out_col = mix( groundColor, skyColor, (1 + dir.y)/2);
	}
	else
	{
		fs_out_col = texture( skyboxTexture, (vs_out_pos) );
	}	
}