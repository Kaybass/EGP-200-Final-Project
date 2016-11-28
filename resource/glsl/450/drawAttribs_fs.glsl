// drawAttribs_fs.glsl
// fragment shader used to visualize incoming attribute data as colors
#version 450

out vec4 fragColor;

in vec4 position_attr;
in vec4 normal_attr;
in vec4 color_attr;
in vec4 texcoord_attr;

void main()
{
//	fragColor = color_attr;

//	fragColor = position_attr;

//	fragColor = normal_attr;
//	fragColor = normal_attr*0.5 + 0.5;

	fragColor = texcoord_attr;
}