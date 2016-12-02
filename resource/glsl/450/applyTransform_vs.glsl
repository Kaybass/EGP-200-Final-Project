// applyTransform_vs.glsl
// vertex shader that multiplies position attribute by mvp; sends result to GL for clipping
#version 430





layout (location = 0) in vec4 position;
layout (location = 4) in vec4 normal;
layout (location = 8) in vec2 texcoord;
//layout (location = 10) in vec2 punty;

layout (location = 0) uniform mat4 mvp;
layout (location = 4) uniform vec2 frame;
out vec2 passtexcoord;
out vec4 normals;
//out vec2 sizeOfTexture;
out vec2 singleImage; 
//out vec2 frame;



void main()
{
	normals = normal;

	singleImage = vec2(0.333, 0.333);

	gl_Position = mvp * position;

	vec2 testcoord = texcoord; 

	testcoord.x += .52;
	testcoord.y += .60;

	testcoord /= .85;

	passtexcoord = testcoord - (singleImage * frame);



	//passtexcoord = texcoord;

}