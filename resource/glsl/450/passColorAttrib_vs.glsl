// passColorAttrib_vs.glsl
// vertex shader that passes color attribute down pipeline
#version 430

layout (location = 0) in vec4 position;
layout (location = 3) in vec4 color;

layout (location = 0) uniform mat4 mvp;

out vec4 passColor;

void main()
{
	gl_Position = mvp * position;

	passColor = color;
}