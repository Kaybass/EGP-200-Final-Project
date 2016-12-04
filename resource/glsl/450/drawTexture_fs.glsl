// drawTexture_fs.glsl by D. Buckstein
// fragment shader that uses a texture coordinate to apply a texture sample


//This file was modified by	Owen Sanders with permission from the author. 

//Owen Sanders: ID# 0972230
//EGP-200-01 - 12/4/2016 - PROJ 3 

//Certificate of Authenticity: “I certify that this work is
//entirely My own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and / or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

//Added the drawTexture from class

#version 450

out vec4 fragColor;

in vec2 passTexcoord;


layout (binding = 0) uniform sampler2D tex; 

void main()
{
	vec4 sampleColor = texture(tex, passTexcoord); 

	vec3 sampleColorNoAlpha = sampleColor.rgb;

	//fragColor = vec4(0.5);


	fragColor = sampleColor;
}
