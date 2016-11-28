// applyTransform_vs.glsl
// vertex shader that multiplies position attribute by mvp; sends result to GL for clipping
#version 430

layout (location = 0) in vec4 position;

layout (location = 0) uniform mat4 mvp;

void main()
{
	gl_Position = mvp * position;
}