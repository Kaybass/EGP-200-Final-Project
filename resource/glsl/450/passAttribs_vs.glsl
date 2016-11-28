// passAttribs_vs.glsl
// vertex shader that passes all incoming attributes down the pipeline
#version 450

layout (location = 0) in vec4 position;
layout (location = 2) in vec4 normal;
layout (location = 3) in vec4 color;
layout (location = 8) in vec4 texcoord;

layout (location = 0) uniform mat4 mvp;

out vec4 position_attr;
out vec4 normal_attr;
out vec4 color_attr;
out vec4 texcoord_attr;

void main()
{
	gl_Position = mvp * position;

	position_attr = position;
	normal_attr = normal;
	color_attr = color;
	texcoord_attr = texcoord;
}