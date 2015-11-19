/////////////////////////////////////////////////////////////////////////
// Vertex shader for the final pass
//
// Copyright 2013 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////
#version 330

uniform mat4 ModelMatrix;  
uniform mat4 ShadowViewMatrix;  //View and proj. matrices are calculated from light's POV
uniform mat4 ShadowProjectionMatrix;
uniform mat4 NormalMatrix;

uniform vec3 lightPos;




in vec4 vertex;
in vec3 vertexNormal;
in vec2 vertexTexture;
in vec3 vertexTangent;

out vec3 tangent;
out vec2 texCoord;
out vec3 worldPos;
out vec3 normalVec, lightVec, eyeVec;
out vec4 position;



void main()
{      
    tangent = vertexTangent;
    texCoord = vertexTexture;



	 

    normalVec = normalize(mat3(NormalMatrix)*vertexNormal);    
    worldPos = (ModelMatrix*vertex).xyz;
   

  
	lightVec = lightPos - worldPos;

    position = ShadowProjectionMatrix*ShadowViewMatrix*ModelMatrix*vertex;
	gl_Position = position;
	
	


}
