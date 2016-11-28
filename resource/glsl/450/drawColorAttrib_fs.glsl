// drawColorAttrib_fs.glsl
// fragment shader that receives color data from previous stage and outputs it
#version 430

out vec4 fragColor;

in vec4 passColor;

void main()
{
	fragColor = passColor;
}