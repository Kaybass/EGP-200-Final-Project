// applyTransform_vs.glsl
// vertex shader that multiplies position attribute by mvp; sends result to GL for clipping
#version 430
//This file was modified with permission from the author. 
//Owen Worked on this file. 
//We completely changed the meaning of this file. It now does uv's for our animated sprite. 

//Owen Sanders: ID# 0972230
//Alex Apmann: ID# 0947096
//EGP-200-01 - 12/4/2016 - PROJ 3 

//Certificate of Authenticity: “I certify that this work is
//entirely My own. The assessor of this project may reproduce this project
//and provide copies to other academic staff, and / or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”




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

	testcoord.x -= .33;
	testcoord.y += .55;

	testcoord /= .80; // manually set it to the first frame... 

	int xint = int(frame.y); 
	xint %= 3; 

	
	int yint = int(frame.y / 3); // want the loss of data

	passtexcoord.x = testcoord.x + (singleImage.x * xint);
	passtexcoord.y = testcoord.y - (singleImage.y * yint);




	//passtexcoord = texcoord;

}