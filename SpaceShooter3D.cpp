//Windows includes - DO NOT EDIT AS LOTS OF INTERDEPENDENCE
#include <math.h>						
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>	//Needed for console output (debugging)
#include <gl/freeglut.h>
#include <iostream>
#include <stdio.h>

#ifdef WIN32
#include "gltools.h"
#include <windows.h>		// Must have for Windows platform builds
#include <gl\gl.h>			// Microsoft OpenGL headers (version 1.1 by themselves)
#include <gl\glu.h>			// OpenGL Utilities
#include "glm.h"
#endif

#include "ShipHud.h"
#include "Jupiter.h"
#include "HoopWithCrystal.h"
#include "Environment.h"

using namespace std;

//  Created by Philip Trwoga on 09/11/2012. Modified on 20/1/16 to include the many VS2015 changes
//  Copyright (c) 2012 __Philip Trwoga__. All rights reserved.
// DO NOT DISTRIBUTE WITHOUT THE AUTHOR'S PERMISSION


//note that this needs gltools.h and gltools.cpp to be included in the shared/library directory

//can define any number of textures here - we just have 2 images
//these are just integers so you can use them as such
#define IMAGE1      0
#define IMAGE2      1
#define IMAGE3      2
#define IMAGE4		3
#define TEXTURE_COUNT 4
GLuint  textures[TEXTURE_COUNT];
GLuint jupiterRotation;
GLint frameRenderTime = 0;
GLint windowWidth = 1066;
GLint windowHeight = 600;
GLfloat zMovement = 0;
GLfloat yMovement = 0;
GLfloat xMovement = 0;
GLfloat zRotation = 0;
GLfloat yRotation = 0;
bool hudOn = 1;
GLfloat hoopsRotation = 0;
int nextHoop = 0;
//below is simply a character array to hold the file names
//note that you may need to replace the below with the full directory root depending on where you put your image files
//if you put them where the exe is then you just need the name as below - THESE IMAGES  ARE IN THE DEBUG FOLDER, YOU CAN ADD ANY NEW ONES THERE 
const char* textureFiles[TEXTURE_COUNT] = { "jupiterC.tga", "hud.tga","starField.tga","tycho_cyl_glow.tga" };

const int hoopCount = 6;
HoopWithCrystal* hoops[hoopCount];

//for lighting if you want to experiment with these
GLfloat mKa[4] = { 0.11f,0.06f,0.11f,1.0f }; //ambient
GLfloat mKd[4] = { 0.43f,0.47f,0.54f,1.0f }; //diffuse
GLfloat mKs[4] = { 1.0f,1.0f,1.0f,1.0f }; //specular
GLfloat mKe[4] = { 0.5f,0.5f,0.0f,1.0f }; //emission

//we need these for the texture loader
//they are to do with the image format and size
GLint iWidth, iHeight, iComponents;
GLenum eFormat;
// this is a pointer to memory where the image bytes will be held 
GLbyte* pBytes0;

bool isCollided(HoopWithCrystal hoop) {
	GLint xMin = hoop.positionX - hoop.radius;
	GLint xMax = hoop.positionX + hoop.radius;
	GLint yMin = hoop.positionY - hoop.radius;
	GLint yMax = hoop.positionY + hoop.radius;
	GLint zMin = hoop.positionZ - 32;
	GLint zMax = hoop.positionZ + 32;
	return (xMovement * -1 >= xMin && xMovement * -1 <= xMax) &&
		(yMovement * -1 >= yMin && yMovement * -1 <= yMax) &&
		(zMovement * -1 >= zMin && zMovement * -1 <= zMax);
}

// Called to draw scene
void RenderScene(void)
{
	// Clear the window with current clearing colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(xMovement, yMovement, zMovement);
	glRotatef(zRotation, 0, 0, 1);
	glRotatef(yRotation, 0, 1, 0);
	Environment env = Environment(textures[IMAGE4]);
	//view the scene
	//gluLookAt(xMovement, yMovement, zMovement,//eye
	//	xMovement, yMovement, 0,//focus
	//	0.00, 1.00, 0.00);//up

	GLfloat matrixMV[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixMV);
	cout << matrixMV[12] << " " << matrixMV[13] << " " << matrixMV[14] << endl;

	Jupiter jupiter = Jupiter(jupiterRotation, textures[IMAGE1]);
	bool nextLoopChanged = false;
	for (int i = 0; i < hoopCount; i++) {
		if (!(*hoops[i]).isHit) {
			(*hoops[i]).drawHoop(textures[IMAGE3], hoopsRotation);
		}
		else if ((*hoops[i]).isHit && i != hoopCount - 1 && nextHoop == i && !nextLoopChanged) {
			(*hoops[i + 1]).makeNext();
			nextHoop++;
			nextLoopChanged = true;
		}
	}
	if (hudOn) {
		ShipHud hud = ShipHud(windowWidth, windowHeight, textures[IMAGE2]);
	}

	glutSwapBuffers();
}


// This function does any needed initialization on the rendering
// context.
void SetupRC()
{
	//textures
	GLuint texture;
	// allocate a texture name
	glGenTextures(1, &texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// photoshop is a good converter to targa (TGA)
	//the gltLoadTGA method is found in gltools.cpp and is orignally from the OpenGL SuperBible book
	//there are quite a few ways of loading images
	// Load textures in a for loop
	glGenTextures(TEXTURE_COUNT, textures);
	//this puts the texture into OpenGL texture memory
 //   glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); - not defined so probably need to update GLEW - not needed here so ignore
	for (int iLoop = 0; iLoop < TEXTURE_COUNT; iLoop++)
	{
		// Bind to next texture object
		glBindTexture(GL_TEXTURE_2D, textures[iLoop]);

		// Load texture data, set filter and wrap modes
		//note that gltLoadTGA is in the glm.cpp file and not a built-in openGL function
		pBytes0 = gltLoadTGA(textureFiles[iLoop], &iWidth, &iHeight,
			&iComponents, &eFormat);

		glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes0);

		//set up texture parameters

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		//try these too
	   // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
		// glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		free(pBytes0);
	}
}


void TimerFunc(int value)
{
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	int deltaT = timeSinceStart - frameRenderTime;
	frameRenderTime = timeSinceStart;
	jupiterRotation += deltaT / 10;
	hoopsRotation += deltaT / 20;

	for (int i = 0; i < hoopCount; i++) {
		if (isCollided(*hoops[i])) {
			(*hoops[i]).isHit = true;
		}		
	}

	glutPostRedisplay();
	glutTimerFunc(25, TimerFunc, 1);
}

void ChangeSize(int w, int h)
{
	GLfloat fAspect;

	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Calculate aspect ratio of the window
	fAspect = (GLfloat)w / (GLfloat)h;

	// Set the perspective coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// field of view of 45 degrees, near and far planes 1.0 and 1000
	//that znear and zfar should typically have a ratio of 1000:1 to make sorting out z depth easier for the GPU
	gluPerspective(55.0f, fAspect, 1.0, 1000.0);

	// Modelview matrix reset
	glMatrixMode(GL_MODELVIEW);
}

void keyboardFuncSpecial(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_UP:
		yMovement += 10.0;
		break;

	case GLUT_KEY_DOWN:
		yMovement -= 10.0;
		break;
	case GLUT_KEY_LEFT:
		zRotation -= 10;
		break;
	case GLUT_KEY_RIGHT:
		zRotation += 10;
		break;
	default:
		break;
	}
}

void keyboardFunc(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'w':
		zMovement += 10;
		break;
	case 's':
		zMovement -= 10;
		break;
	case 'a':
		xMovement -= 10;
		break;
	case 'd':
		xMovement += 10;
		break;
	case 'z':
		yRotation += 10;
		break;
	case 'x':
		yRotation -= 10;
		break;
	case 'h':
		hudOn = !hudOn;
		break;
	default:
		break;
	}
}

void setupHoops() {
	for (int i = 0; i < hoopCount; i++) {
		hoops[i] = new HoopWithCrystal(40 + (i * 10), 50 + (i * 10), -350 - (i * 10), 50);
	}
	(*hoops[0]).makeNext();
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Spaceshooter3D");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutKeyboardFunc(keyboardFunc);
	glutSpecialFunc(keyboardFuncSpecial);
	glutTimerFunc(25, TimerFunc, 1);
	SetupRC();
	setupHoops();
	glutMainLoop();
	return 0;
}






