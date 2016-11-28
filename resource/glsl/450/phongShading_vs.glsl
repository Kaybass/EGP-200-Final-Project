// phongShading_vs.glsl
// vertex shader that computes vertex-related values 
//	for phong and related shading models
#version 450


layout (location = 0) in vec4 position;
layout (location = 2) in vec4 normal;

layout (location = 0) uniform mat4 mvp;
layout (location = 16)uniform vec4 lightPos;
layout (location = 20)uniform vec4 eyePos;




//vertex lighting var
//out vec3 shading;

out vec4 N;
out vec4 L;
out vec4 V;
//not feasible out vec4 R;

void main()
{

	gl_Position = mvp * position;

	N = normal;
	
	L = lightPos - position;

	V = eyePos - position;

}