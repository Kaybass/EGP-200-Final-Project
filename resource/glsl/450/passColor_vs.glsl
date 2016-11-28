// passColor_vs.glsl
// vertex shader that passes the incoming color down the pipeline
#version 450

layout (location = 0) in vec4 position;

layout (location = 3) in vec4 color;

out vec4 passColor;

void main()
{
	gl_Position = position;

	passColor = color;
}