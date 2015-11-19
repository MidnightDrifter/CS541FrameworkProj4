///////////////////////////////////////////////////////////////////////
// Provides the framework for graphics projects.  Most of this small
// file contains the GLUT calls needed to open a window and hook up
// various callbacks for mouse/keyboard interaction and screen resizes
// and redisplays.
//
// Copyright 2013 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
    // Includes for Windows
    #include <windows.h>
    #include <cstdlib>
    #include <fstream>
    #include <limits>
    #include <crtdbg.h>
#else
    // Includes for Linux
#endif

#include <glload/gl_3_3.h>
#include <glload/gl_load.hpp>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
using namespace glm;

#include "scene.h"
#include "AntTweakBar.h"

Scene scene;

// Some globals used for mouse handling.
int mouseX, mouseY;
bool leftDown = false;
bool middleDown = false;
bool rightDown = false;
bool shifted;

////////////////////////////////////////////////////////////////////////
// Called by GLUT when the scene needs to be redrawn.
void ReDraw()
{
	float rx =  (scene.width * scene.ry) / (scene.height);

	if (scene.canMove)
	{
		scene.WorldView = Translate(-1 * scene.eyePos.x, -1 * scene.eyePos.y, -1 * scene.eyePos.z) * Rotate(0, scene.tilt - 90)*Rotate(2, scene.spin);
		//scene.WorldView = scene.WorldView * Rotate(2, scene.spin);

	}
	else
	{
		scene.WorldView = Translate(scene.tx, scene.ty, -1 * scene.zoom) * Rotate(0, scene.tilt - 90) * Rotate(2, scene.spin);
		//scene.WorldView = scene.WorldView * Rotate(2, scene.spin);

	//	scene.WorldView = Translate(scene.tx, scene.ty, -1 * scene.zoom);
	//	scene.WorldView = scene.WorldView * Rotate(0, scene.tilt - 90);
//	scene.WorldView = scene.WorldView * Rotate(2, scene.spin);
	}
	scene.WorldProj = Perspective(rx, scene.ry, scene.front, scene.back);


    scene.DrawScene();
    TwDraw();
    glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////
// Called by GLUT when the window size is changed.
void ReshapeWindow(int w, int h)
{
    if (w && h) {
        glViewport(0, 0, w, h); }
    TwWindowSize(w,h);
    scene.width = w;
    scene.height = h;

    // Force a redraw
    glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////
// Called by GLut for keyboard actions.
void KeyboardDown(unsigned char key, int x, int y)
{
	if (TwEventKeyboardGLUT(key, x, y)) return;

	scene.prevTime = scene.currTime;
	scene.currTime = time(&scene.currTime);
	scene.diff = difftime(scene.currTime, scene.prevTime);
	scene.speed = scene.speed*.9*scene.diff;

	switch (key) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		scene.mode = key - '0';
		glutPostRedisplay();
		break;



	case 'w':
		scene.wDown = true;
	case 's':
		scene.sDown = true;
	case 'a':
		scene.aDown = true;
	case 'd':
		scene.dDown = true;
	case 27:                    // Escape key
	case 'q':




		if (scene.canMove)
		{
			if (scene.wDown)
			{
				scene.eyePos += scene.speed*(sin((scene.spin)*PI / 180), cos(scene.spin*PI / 180), 0);
			}
			if (scene.aDown)
			{
				scene.eyePos += scene.speed*((-1 * cos(scene.spin*PI / 180)), sin((scene.spin)*PI / 180), 0);
			}
			if (scene.sDown)
			{
				scene.eyePos -= scene.speed*(sin((scene.spin)*PI / 180), cos(scene.spin*PI / 180), 0);

			}
			if (scene.dDown)
			{
				scene.eyePos -= scene.speed*((-1 * cos(scene.spin*PI / 180)), sin((scene.spin)*PI / 180), 0);
			}
		}
		// exit(0);
	}

}

void KeyboardUp(unsigned char key, int x, int y)
{

	switch (key)
	{
	case GLUT_ACTIVE_SHIFT:
		scene.canMove = (!scene.canMove);
	case 'w':
		scene.wDown = false;
	case 's':
		scene.sDown = false;
	case 'a':
		scene.aDown = false;
	case 'd':
		scene.dDown = false;
	}
	if (TwEventKeyboardGLUT(key, x, y)) return;

}

////////////////////////////////////////////////////////////////////////
// Called by GLut when a mouse button changes state.
void MouseButton(int button, int state, int x, int y)
{
	//if (TwEventMouseButtonGLUT(button, state, x, y)) return;

	// To determine if the shift was pressed:
	// if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) ...

	// To determine which button changed state:
	// if (button == GLUT_LEFT_BUTTON)
	// if (button == GLUT_MIDDLE_BUTTON)
	// if (button == GLUT_RIGHT_BUTTON)

	// To determine its new state:
	// if (state == GLUT_UP)
	// if (state == GLUT_DOWN)




	if (glutGetModifiers() && GLUT_ACTIVE_SHIFT)
	{
		shifted = true;
	}

	else
	{
		shifted = false;
	}


	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == (GLUT_UP))
		{
			leftDown = false;
		}

		else if (state == GLUT_DOWN)
		{
			leftDown = true;
		}
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == (GLUT_UP))
		{
			rightDown = false;
		}

		else if (state == GLUT_DOWN)
		{
			rightDown = true;
		}

	}

	else if (button == 5 || button == 6)
	{
		if (button == 5)
		{
			scene.zoom += .10;
		}
		else
		{
			scene.zoom -= .10;
		}

	}



	glutPostRedisplay();
	if (TwEventMouseButtonGLUT(button, state, x, y)) return;
}

////////////////////////////////////////////////////////////////////////
// Called by GLut when a mouse moves (while a button is down)
void MouseMotion(int x, int y)
{
	if (TwEventMouseMotionGLUT(x, y)) return;

	//(x, y) = current position on screen
	if (!scene.canMove)
	{

		if (leftDown)
		{
			//Left right motion, left increases, right decreases spin.

			float tempX, tempY;
			tempX = x - scene.curMouseX;
			tempY = y - scene.curMouseY;

			if (tempY > 0)  //Moved up, increase tilt
			{
				scene.tilt += y;
			}
			if (tempY < 0)  //Moved dowwn, decrease tilt
			{
				scene.tilt -= y;
			}

			if (tempX >0)
			{
				scene.spin -= x;
			}

			if (tempX < 0)
			{
				scene.spin += x;
			}


			scene.curMouseX = x;
			scene.curMouseY = y;


			//scene.spin -= (x / 100.0);

			//Up down motion, up increases, down decreases tilt
			//scene.tilt += (y / 100.0);



		}
		else if (rightDown)
		{
			//Sliding motion, changes tx and ty
			float tempX, tempY;
			tempX = x - scene.curMouseX;
			tempY = y - scene.curMouseY;

			if (tempY > 0)  //Moved up, increase tilt
			{
				scene.ty += y;
			}
			if (tempY < 0)  //Moved dowwn, decrease tilt
			{
				scene.ty -= y;
			}

			if (tempX >0)
			{
				scene.tx += x;
			}

			if (tempX < 0)
			{
				scene.tx -= x;
			}





			//scene.tx += x/1000.0;
			//scene.ty += y/1000.0;

		}

		else
		{
			//Do nothing
		}
	}

	//if (TwEventMouseMotionGLUT(x,y)) return;

}

////////////////////////////////////////////////////////////////////////
// Functions called by AntTweakBar
void Quit(void *clientData)
{
    TwTerminate();
    glutLeaveMainLoop();
}

void ToggleGround(void *clientData)
{
    scene.drawGround = !scene.drawGround;
}

void ToggleSpheres(void *clientData)
{
    scene.drawSpheres = !scene.drawSpheres;
}

void TW_CALL SetModel(const void *value, void *clientData)
{
    int i = *(int*)value; // AntTweakBar forces this cast.
    scene.SetCentralModel(i);
}

void TW_CALL GetModel(void *value, void *clientData)
{
    *(int*)value = scene.centralModel;
}

////////////////////////////////////////////////////////////////////////
// Do the OpenGL/GLut setup and then enter the interactive loop.
int main(int argc, char** argv)
{
    // Initialize GLUT and open a window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitContextVersion (3, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
    glutInitWindowSize(750,750);
    glutCreateWindow("Class Framework");
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

    // Initialize OpenGl
    glload::LoadFunctions();
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("Rendered by: %s\n", glGetString(GL_RENDERER));
    fflush(stdout);

    // Hookup GLUT callback for all events we're interested in
    glutIgnoreKeyRepeat(true);
    glutDisplayFunc(&ReDraw);
    glutReshapeFunc(&ReshapeWindow);
    glutKeyboardFunc(&KeyboardDown);
    glutKeyboardUpFunc(&KeyboardUp);
    glutMouseFunc(&MouseButton);
    glutMotionFunc(&MouseMotion);
    glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
    glutSpecialFunc((GLUTspecialfun)TwEventSpecialGLUT);

    // Initialize the tweakbar with a few tweaks.  
    TwInit(TW_OPENGL, NULL);
    TwGLUTModifiersFunc((int(TW_CALL*)())glutGetModifiers);
    TwBar *bar = TwNewBar("Tweaks");
    TwDefine(" Tweaks size='200 300' ");
    TwAddButton(bar, "quit", (TwButtonCallback)Quit, NULL, " label='Quit' key=q ");

    TwAddVarCB(bar, "centralModel", TwDefineEnum("CentralModel", NULL, 0),
               SetModel, GetModel, NULL,
               " enum='0 {Teapot}, 1 {Bunny}, 2 {Dragon}, 3 {Sphere}' ");
    TwAddButton(bar, "Spheres", (TwButtonCallback)ToggleSpheres, NULL, " label='Spheres' ");
    TwAddButton(bar, "Ground", (TwButtonCallback)ToggleGround, NULL, " label='Ground' ");
    
    // Initialize our scene
    scene.InitializeScene();

    // Enter the event loop.
    glutMainLoop();
}
