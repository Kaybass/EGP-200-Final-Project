// drawSolidColor_fs.glsl
// fragment shader that copies a uniform color to the output
#version 430

//This file was modified with permission from the author. 
//Owen Worked on this file. 
//We completely changed the meaning of this file. It no longer draws solid colors, but draws animated sprites using instructuis sent from the vs. 

//Owen Sanders: ID# 0972230
//Alex Apmann: ID# 0947096
//EGP-200-01 - 12/4/2016 - PROJ 3 

//Certificate of Authenticity: “I certify that this work is
//entirely My own. The assessor of this project may reproduce this project
//and provide copies to other academic staff, and / or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

out vec4 fragColor;

//layout (location = 16) uniform vec4 solidColor;

in vec2 passtexcoord;
in vec4 normals;
in vec2 sizeOfTexture;
in vec2 finalSize; 
in vec2 singleImage;
//in vec2 frame;

layout (binding = 0) uniform sampler2D sprite;

void main()
{

	

	vec2 testCoord; 

	//testCoord.x = passtexcoord.x - .43; 
	//testCoord.y = passtexcoord.y + .24; 

	//testCoord = passtexcoord / .33;

	vec4 final = texture(sprite, passtexcoord);

	fragColor = final;



	fragColor.a = 1.0;

}