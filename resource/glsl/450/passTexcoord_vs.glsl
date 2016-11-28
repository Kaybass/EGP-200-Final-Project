// passTexcoord_vs.glsl
// vertex shader that passes texture coordinates down the pipeline
#version 450


//attributes
layout (location = 0) in vec4 position;

layout (location = 8) in vec2 texcoord;

layout (location = 0) uniform mat4 mvp;

out vec2 passtexcoord;

void main()
{
	gl_Position = mvp * position;

//	passtexcoord = texcoord;

	passtexcoord = texcoord;
}