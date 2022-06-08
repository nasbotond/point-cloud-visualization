#version 330 core

in vec3 fColor;
in vec2 gs_out_tex;

out vec4 fs_out_col;

uniform sampler2D texImage;

void main()
{
	// fs_out_col = vec4(0.0, 1.0, 0.0, 1.0);
	// fs_out_col = vec4(fColor, 1.0);
	fs_out_col = texture(texImage, gs_out_tex);
	/*
	if(vs_out_texOrCol == 0.0)
	{
		fs_out_col = vec4(vs_out_col, 1);
	}
	else
	{
		fs_out_col = texture(texImage, vs_out_tex);
	}
	*/
}