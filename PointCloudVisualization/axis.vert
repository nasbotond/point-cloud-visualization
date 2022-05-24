#version 330 core

// variables going forward through the pipeline
out vec3 vs_out_col;

// external parameters of the shader
uniform mat4 MVP;

vec3 positions[6] = vec3[6](
	vec3(0, 0, 0),
	vec3(1, 0, 0),
	vec3(0, 0, 0),
	vec3(0, 1, 0),
	vec3(0, 0, 0),
	vec3(0, 0, 1)
);

vec3 colors[6] = vec3[6](
	vec3(1, 0, 0),
	vec3(1, 0, 0),
	vec3(0, 1, 0),
	vec3(0, 1, 0),
	vec3(0, 0, 1),
	vec3(0, 0, 1)
);

void main()
{
	gl_Position = MVP * vec4( positions[gl_VertexID], 1 );	// [x,y,w,w] => after homogeneous division [x/w, y/w, 1]

	vs_out_col = colors[gl_VertexID];
}