// drawSolidColor_fs.glsl
// fragment shader that copies a uniform color to the output
#version 430

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