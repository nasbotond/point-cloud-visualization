#version 330 core

// per-fragment attributes coming from the pipeline
in vec2 vs_out_tex;
in vec3 vs_out_col;
in float vs_out_texOrCol;

out vec4 fs_out_col;

uniform sampler2D texImage;

void main()
{
	if(vs_out_texOrCol == 0.0)
	{
		fs_out_col = vec4(vs_out_col, 1);
	}
	else
	{
		fs_out_col = texture(texImage, vs_out_tex);
	}
}