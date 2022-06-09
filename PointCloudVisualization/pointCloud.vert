#version 330 core

// variables coming from the VBO
in vec3 vs_in_pos;
in vec3 vs_in_c1;
in vec3 vs_in_c2;
in vec2 vs_in_t1;
in vec2 vs_in_t2;
in vec3 vs_in_norm;

// variables going forward through the pipeline
out vec3 vs_out_col;
out vec2 vs_out_tex;
out float vs_out_texOrCol; // 0 -> color, 1 -> texture

// external parameters of the shader
uniform mat4 MVP;
uniform int colOrTexIndex; // c1: 1, c2: 2, t1: 3, t2: 4

void main()
{
	gl_Position = MVP * vec4( vs_in_pos, 1 );

	switch (colOrTexIndex) 
	{
		case 0:
			vs_out_col = vs_in_c1;
			vs_out_tex = vs_in_t1;
			vs_out_texOrCol = 0.0;
			break;
		case 1:
			vs_out_col = vs_in_c2;
			vs_out_tex = vs_in_t1;
			vs_out_texOrCol = 0.0;
			break;
		case 2:
			vs_out_col = vs_in_c1;
			vs_out_tex = (1.0-vs_in_t1);
			vs_out_texOrCol = 1.0;
			break;
		case 3:
			vs_out_col = vs_in_c1;
			vs_out_tex = (1.0-vs_in_t2);
			vs_out_texOrCol = 1.0;
			break;
	}
}