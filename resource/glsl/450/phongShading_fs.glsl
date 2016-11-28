// phongShading_fs.glsl
// fragment shader that computes phong shading based on 
//	light ray and eye vector received from vert shader
#version 450


out vec4 fragColor;

const vec3 ambience = vec3(0.01, 0.0, 0.0);

//in vec3 shading;

in vec4 N;
in vec4 L;
in vec4 V;


void main()
{
	vec4 l = normalize(L);
	vec4 n = normalize(N);
	vec4 v = normalize(V);

	//diffuse
	float diffuse = dot(n,l);

	vec4 r = (diffuse + diffuse)*n-l;
	

	float specular = dot(v,r);

	diffuse  = max(0.0,diffuse);
	specular = max(0.0,specular);
	specular *= specular;
	specular *= specular;
	specular *= specular;
	

	//fragColor = vec3(diffuse + specular) + 
	vec3 phong = ambience;

	fragColor.rgb = phong;
	fragColor.a = 1.0;
}