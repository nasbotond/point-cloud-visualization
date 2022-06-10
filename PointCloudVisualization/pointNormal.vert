#version 330 core

// variables coming from the VBO
in vec3 vs_in_pos;
in vec3 vs_in_c1;
in vec3 vs_in_c2;
in vec2 vs_in_t1;
in vec2 vs_in_t2;
in vec3 vs_in_norm;

out VS_OUT {
    vec3 normal;
	int magnitude;
} vs_out;

// external parameters of the shader
uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;
uniform int normal_magnitude;
uniform vec3 direction;

void main()
{
	gl_Position = MVP * vec4( vs_in_pos, 1 );

	float dir = dot(vs_in_norm, direction-vs_in_pos);

	vs_out.normal = normalize(vec3(worldIT * vec4(vs_in_norm, 0)));

	// make all normal vectors point in the same direction
	if(dir < 0)
	{
		vs_out.normal = normalize(vec3(worldIT * vec4(-1*vs_in_norm, 0)));
	}

	vs_out.magnitude = normal_magnitude;
}