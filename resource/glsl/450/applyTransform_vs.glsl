// applyTransform_vs.glsl
// vertex shader that multiplies position attribute by mvp; sends result to GL for clipping
#version 430





layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 8) in vec2 texcoord;

layout (location = 0) uniform mat4 mvp;
out vec2 passtexcoord;
out vec4 normals;



void main()
{
	normals = normal;

	passtexcoord = texcoord;

	gl_Position = mvp * position;
}