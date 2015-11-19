/////////////////////////////////////////////////////////////////////////
// Pixel shader for the final pass
//
// Copyright 2013 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////
#version 330

uniform int mode;               // 0..9, used for debugging
uniform bool direct;            // Direct color -- no lighting calculation

uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

uniform vec3 lightValue, lightAmbient;

uniform sampler2D groundTexture;
uniform sampler2D tex;
uniform sampler2D earthBaseTexture;
uniform sampler2D redEarthTexture;




in vec3 normalVec, lightVec;
in vec3 eyeVec;
in vec2 texCoord;
in vec3 worldPos;
in vec4 position;



float PI = 3.14159;

float LN(vec3 light, vec3 normal)
{
return max(dot(light, normal), 0.0);
}





vec3 BRDF(vec3 eye, vec3 normal, vec3 light, vec3 dif, vec3 spec, float shiny)
{

float alpha = pow(8192, shiny);



	vec3 V = normalize(eye);
    vec3 N = normalize(normal);
    vec3 L = normalize(light);
	vec3 H = normalize(L+V);

	float LN = max(dot(L,N), 0.0);
	float HN = max(dot(H,N), 0.0);
	float LH = max(dot(L,H), 0.0);

	vec3 mySpec;// = (1-specular.x, 1-specular.y, 1-specular.z); 
	mySpec.x = 1-spec.x;
	mySpec.y=1-spec.y;
	mySpec.z=1-spec.z;
	vec3 F = spec + (mySpec)*(pow((1-LH), 5));
	float D = ((alpha+2)/(2*PI))*(pow(HN, alpha)); 
	
	//Using approx of G() / (LN * VN) = approx  1/(LH*LH)

	vec3 BRDF = (F*D)/(4*LH*LH) + (dif / PI);
	
	return BRDF;


}


vec3 BRDF( vec3 dif, vec3 spec, float shiny)
{
return BRDF(eyeVec, normalVec, lightVec, dif, spec, shiny);
}

void main()
{

gl_FragColor = position;	   
}