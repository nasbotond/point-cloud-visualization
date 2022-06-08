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
	int l;
	int w;
	vec3 color;
	vec2 tex;
} vs_out;

// external parameters of the shader
uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;
uniform int l; // length
uniform int w; // width
uniform int rectColor;

void main()
{
	gl_Position = MVP * vec4( vs_in_pos, 1 );

	vs_out.normal = normalize(vec3(worldIT * vec4(vs_in_norm, 0)));
	vs_out.l = l;
	vs_out.w = w;

	if(rectColor == 1)
	{
		vs_out.color = vs_in_c1;
	}
	else
	{
		vs_out.color = vs_in_c2;
	}

	vs_out.tex = vs_in_t1;
}