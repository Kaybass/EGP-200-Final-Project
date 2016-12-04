// passTexcoord_vs.glsl by D. Buckstein
// vertex shader that passes texture coordinates down the pipeline
#version 450


//This file was modified by	Owen Sanders with permission from the author. 

//Owen Sanders: ID# 0972230
//EGP-200-01 - 12/4/2016 - PROJ 3 

//Certificate of Authenticity: “I certify that this work is
//entirely My own. The assessor of this project may reproduce this project
//and provide copies to other academic staff, and / or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

//Added the texcoord from class. 
layout (location = 0) in vec4 position;
layout (location = 8) in vec2 texcoord;


layout (location = 0) uniform mat4 mvp;


out vec2 passTexcoord;


void main()
{
	gl_Position = mvp * position;

	passTexcoord = texcoord* vec2(1);
}