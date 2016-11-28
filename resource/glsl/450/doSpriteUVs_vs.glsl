// doSpriteUVs_vs.glsl
// vertex shader that prepares texture coordinates for sprite rendering
// default fragment shader is the same as drawing texture!
#version 450


// ATTRIBUTES: 
layout (location = 0) in vec4 position;

layout (location = 8) in vec2 texcoord;

layout (location = 0) uniform mat4 mvp;

layout (location = 16) uniform vec2 frameSize;

layout (location = 18) uniform vec2 currentFrame;

out vec2 passtexcoord;


void main()
{
	gl_Position = mvp * position;

	passtexcoord = frameSize * (texcoord + currentFrame);
}