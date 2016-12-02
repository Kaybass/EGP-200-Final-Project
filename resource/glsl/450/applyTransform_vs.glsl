// applyTransform_vs.glsl
// vertex shader that multiplies position attribute by mvp; sends result to GL for clipping
#version 430





layout (location = 0) in vec4 position;
layout (location = 4) in vec4 normal;
layout (location = 8) in vec2 texcoord;

layout (location = 0) uniform mat4 mvp;
out vec2 passtexcoord;
out vec4 normals;
out vec2 sizeOfTexture;
out vec2 singleImage; 



void main()
{
	normals = normal;

	passtexcoord = texcoord;

	//sizeOfTexture = vec2(768, 768);
	

	singleImage = vec2(0.333, 0.333);

	gl_Position = mvp * position;

	//gl_PointSize = 256 / position.w;
}