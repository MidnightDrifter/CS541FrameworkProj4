////////////////////////////////////////////////////////////////////////
// The scene class contains all the parameters needed to define and
// draw the (really) simple scene, including:
//   * Models (in a VAOs as)
//   * Light parameters
//   * Surface properties
//   * Viewport size parameters
//   * Viewing transformation values
//   * others ...
//
// Some of these parameters are set when the scene is built, and
// others are set by the framework in response to user mouse/keyboard
// interactions.  All of them may be used to draw the scene.

#include <glm/glm.hpp>
#include <glm/ext.hpp>
using namespace glm;

#include "models.h"
#include "shader.h"
#include "texture.h"
#include "fbo.h"

class Scene
{
public:
    // Some user controllable parameters
    int mode;  // Communicated to the shaders as "mode".  Keys '0'-'9'
    int nSpheres;
    bool drawSpheres;
    bool drawGround;

    int centralType;
    int centralModel;
    MAT4 centralTr;

    // Viewing transformation parameters
    // Fixme:  This is a good place to define your viewing parameters, spin, tilt, ...
    

    // Light colors and position parameters;  Mouse buttons SHIFT 1-3
    vec3 ambientColor;
    vec3 lightColor;
    float lightSpin;
    float lightTilt;
    vec3 lightDir;
    float lightDist;
    


    // Viewport
    int width, height;
	float widthf, heightf;
    // Shader programs
    ShaderProgram lightingShader;

    // The polygon models (VAOs - Vertex Array Objects)
    Model* centralPolygons;
    Model* spherePolygons;
    Model* groundPolygons;

    // Texture
    Texture groundTexture;


    // Main methods
    void InitializeScene();
    void DrawScene();

    // Helper methods
    void SetCentralModel( const int i);
    void DrawSun(unsigned int program, MAT4& ModelTr);
    void DrawSpheres(unsigned int program, MAT4& ModelTr);
    void DrawGround(unsigned int program, MAT4& ModelTr);






	//WorldView and WorldProj variables
	float tilt = -90, spin = 0, tx = 0, ty = 0, zoom = 150, ry = 0.2, front = 0.1, back = 1000;
	//= 0.2;   //ry* (width / height);
	bool canMove = false, wDown = false, sDown = false, aDown = false, dDown = false;
	time_t prevTime, currTime;
	float speed = 1;
	float diff;
	vec3 eyePos =vec3(0, 230, 3);
//	float eyePosX=0.0, eyePosY=-250, eyePosZ=4;
	
	float TRY = 0.4f, TRX = 0.4f, Tfront = 0.1, Tback = 1000.0f, tCenterX = 0.0f, tCenterY = 0.0f, tCenterZ = 0.0f;
	vec3 tCenter;
	
	ShaderProgram shadowShader;
	FBO shadowMap;
	vec3 centerOfScene = vec3(0.0f, 0.0f, 0.0f);

	MAT4 WorldView = MAT4();
	MAT4 WorldProj = MAT4();
	MAT4 rightButtonTranslate = MAT4();
	MAT4 translate = MAT4();
	float curMouseX, curMouseY;

	//MAT4 WorldViewShadow = MAT4();
	//MAT4 WorldProjShadow = MAT4();
	vec3 upDirection = vec3(0.0f, 0.0f, 1.0f);

	bool shadowOffsetBoolean = true;
	float shadowOffset = 0.5f;


	


};

