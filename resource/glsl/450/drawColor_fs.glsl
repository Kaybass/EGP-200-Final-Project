// drawColor_fs.glsl
// fragment shader that receives a colour and applies it to the fragment
#version 450

out vec4 fragColor;

in vec4 passColor;

void main()
{
	fragColor = passColor;
}