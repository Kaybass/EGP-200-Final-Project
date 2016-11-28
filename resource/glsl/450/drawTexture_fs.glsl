// drawTexture_fs.glsl
// fragment shader that uses a texture coordinate to apply a texture sample
#version 450

out vec4 fragColor;

in vec2 passtexcoord;

layout (binding = 0) uniform sampler2D teximage;

void main()
{
	vec4 samplecolor = texture(teximage, passtexcoord);

	fragColor = samplecolor;
}