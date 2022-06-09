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
	float texOrCol; // 0 -> color, 1 -> texture
} vs_out;

// external parameters of the shader
uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;
uniform int l; // length
uniform int w; // width
uniform int rectColOrTex;
uniform vec3 direction;

void main()
{
	float dir = dot(vs_in_norm, direction-vs_in_pos);
	gl_Position = vec4( vs_in_pos, 1 );

	vs_out.normal = normalize(vec3(worldIT * vec4(vs_in_norm, 0)));

	if(dir < 0)
	{
		vs_out.normal = normalize(vec3(worldIT * vec4(-1*vs_in_norm, 0)));
	}

	vs_out.l = l;
	vs_out.w = w;

	if(rectColOrTex == 0)
	{
		vs_out.color = vs_in_c1;
		vs_out.tex = vs_in_t1;
		vs_out.texOrCol = 0.0;
	}
	else if(rectColOrTex == 1)
	{
		vs_out.color = vs_in_c2;
		vs_out.tex = vs_in_t1;
		vs_out.texOrCol = 0.0;
	}
	else if(rectColOrTex == 2)
	{
		vs_out.color = vs_in_c2;
		vs_out.tex = (1.0-vs_in_t1);
		vs_out.texOrCol = 1.0;
	}
	else
	{
		vs_out.color = vs_in_c2;
		vs_out.tex = (1.0-vs_in_t2);
		vs_out.texOrCol = 1.0;
	}

	
}