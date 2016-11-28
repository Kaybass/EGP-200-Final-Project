// passThru_fs.glsl
// pass-thru fragment shader
#version 450

out vec4 fragColor;

const vec4 white = vec4(1.0);

void main()
{
	fragColor = white;
}