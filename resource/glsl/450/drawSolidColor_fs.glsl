// drawSolidColor_fs.glsl
// fragment shader that copies a uniform color to the output
#version 430

out vec4 fragColor;

layout (location = 16) uniform vec4 solidColor;

void main()
{
	fragColor = solidColor;
}