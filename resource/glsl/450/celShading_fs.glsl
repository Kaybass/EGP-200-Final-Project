// celShading_fs.glsl
// fragment shader that computes cel shading based on 
//	light ray and ramp texture
#version 450


out vec4 fragColor;

const vec3 ambience = vec3(0.01, 0.0, 0.0);

//in vec3 shading;

in vec4 N;
in vec4 L;
in vec4 V;

layout (binding = 1) uniform sampler2D ramp;

vec2 rampCoord = vec2(0.0);


void main()
{
	vec4 l = normalize(L);
	vec4 n = normalize(N);
	vec4 v = normalize(V);

	//diffuse
	float diffuse  = dot(n,l);
	//float diffuse2 = diffuse*0.5 + 0.5;

	vec4 R = (diffuse + diffuse)*n - l;

	float specular = dot(V,R);
	
	
	specular *= specular;
	specular *= specular;
	specular *= specular;

	rampCoord.x = diffuse*0.5 + 0.5;

	vec4 rampSample = texture(ramp, rampCoord);

	rampCoord.x = specular*0.5 + 0.5;

	vec4 rampSampleSpecular = texture(ramp, rampCoord);


	fragColor = rampSample * rampSampleSpecular;
	
	vec4 r = (diffuse + diffuse)*n-l;
}