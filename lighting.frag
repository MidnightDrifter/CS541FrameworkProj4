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

uniform sampler2D shadowMap;
uniform float shadowOffset;


in vec3 normalVec, lightVec;
in vec3 eyeVec;
in vec2 texCoord;
in vec3 worldPos;
in vec4 shadowCoord;



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
	

	vec3 BRDF = (F*D)/(4*LH*LH) + (dif / PI);
	
	return BRDF;


}


vec3 BRDF( vec3 dif, vec3 spec, float shiny)
{
return BRDF(eyeVec, normalVec, lightVec, dif, spec, shiny);
}

highp float rand(vec2 co)
{
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt= dot(co.xy ,vec2(a,b));
    highp float sn= mod(dt,3.14);
    return fract(sin(sn) * c);
}

void main()
{



float PI = 3.14159;

    vec3 N = normalize(normalVec);
    vec3 L = normalize(lightVec);
	

	float LN = max(dot(L,N), 0.0);
	
	
vec3 t = BRDF(eyeVec, normalVec, lightVec, diffuse, specular, shininess);
vec3 output = t * LN * lightValue + lightAmbient;	

float LNReal = dot(L,N);
vec3 white = vec3(1.0f, 1.0f, 1.0f);
vec3 black = vec3(0.0f, 0.0f, 0.0f);

vec2 shadowTexCoord = shadowCoord.xy/shadowCoord.w;





	    if (textureSize(groundTexture,0).x>1) // Is the texture defined?
        {
		
		vec3 temp = BRDF(texture(groundTexture,2.0*texCoord.st).xyz, specular, shininess);
		gl_FragColor.xyz = temp + lightAmbient;
		
		
		
		
		}
		



		else if(shadowCoord.w>0 && (shadowTexCoord.x >= 0 && shadowTexCoord.x <=1) &&(shadowTexCoord.y >=0 && shadowTexCoord.y <=0))
			//else if((shadowTexCoord.x >= 0 && shadowTexCoord.x <=1) &&(shadowTexCoord.y >=0 && shadowTexCoord.y <=0))
				{
		
							//Light depth = texture value at ( texture(shadowMap, shadowIndex).w)
							//Pixel depth = shadowCoord.w
							//If pixel depth > light depth, pixel is in shadow, only apply ambient lighting

							
							//if(shadowCoord.w > texture(shadowMap, shadowTexCoord.xy).w)
							//if((shadowCoord.z - texture(shadowMap, shadowTexCoord).z) >= shadowOffset )
							if(shadowCoord.w - shadowOffset  > texture(shadowMap, shadowTexCoord.xy).w)
							{
								gl_FragColor.xyz = lightAmbient;	
							}
				
			
							else
							{
								gl_FragColor.xyz = output;
							}
							
					}

		
					
					
	
		
		else
		 { 
		 //gl_FragColor.xyz = vec3(shadowCoord.w, shadowCoord.w , shadowCoord.w );
		 gl_FragColor.xyz =output;
			
		 }
	  



	 /*

	if( shadowTexCoord.x >=0 && shadowTexCoord.x <=1 && shadowTexCoord.y>=0 && shadowTexCoord.y <=1)
	{
		gl_FragColor.xyz = vec3(1.0, 0.5, 0.0);
	}
	*/

	//if(shadowCoord.w >0)
	//{gl_FragColor.xyz=vec3(1.0, 0.5, 0.0);}

	// gl_FragColor.xy = shadowCoord.xy/(shadowCoord.z);
	 

	   


	 // gl_FragColor.xyz = vec3(shadowCoord.w, shadowCoord.w, shadowCoord.w);
	 // gl_FragColor = shadowCoord;
	 // gl_FragColor.xyz = vec3(texture(shadowMap, shadowTexCoord).w,texture(shadowMap, shadowTexCoord).w,texture(shadowMap, shadowTexCoord).w);



	// gl_FragColor = texture(shadowMap, (gl_FragCoord.xy / 2048.0f))/100.0f;
	vec2 uv = gl_FragCoord.xy / vec2(2048.0f, 2048.0f);
	gl_FragColor.xyz = texture(shadowMap, uv).xyz / 100.0f;
	float r = rand(gl_FragCoord.xy);
	vec2 random = (vec2(r, r) + vec2(1.0f,1.0f))/2.0f;
	gl_FragColor.xyz = texture(shadowMap, vec2(0.5f,0.5f)).xyz / 100.0f;

}