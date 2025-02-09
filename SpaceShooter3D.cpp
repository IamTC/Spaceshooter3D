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

#include <random>
#include "ShipHud.h"
#include "Jupiter.h"
#include "HoopWithCrystal.h"
#include "Environment.h"
#include "src/EulerAngles.h"
#include "src/Quaternion.h"
#include "src/RotationMatrix.h"

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
GLfloat zMovementEnd = 0;
GLfloat yMovementEnd = 0;
GLfloat xMovementEnd = 0;

GLfloat zRotation = 0;
GLfloat yRotation = 0;
GLfloat xRotation = 0;
GLfloat zRotationEnd = 0;
GLfloat yRotationEnd = 0;
GLfloat xRotationEnd = 0;

bool hudOn = 1;
GLfloat hoopsRotation = 0;
int nextHoop = 0;

// Globals.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Euler angles.
static float matrixData[16]; // Rotation matrix values.
static float t = 0.0; // Interpolation parameter.

static Quaternion identityQuaternion(1.0, 0.0, 0.0, 0.0), q; // Global identity quaternion.
static Quaternion rotationQuaternion(1.0, 0.0, 0.0, 0.0); // Global identity quaternion.
static EulerAngles e; // Global Eular angle value.

//below is simply a character array to hold the file names
//note that you may need to replace the below with the full directory root depending on where you put your image files
//if you put them where the exe is then you just need the name as below - THESE IMAGES  ARE IN THE DEBUG FOLDER, YOU CAN ADD ANY NEW ONES THERE 
const char* textureFiles[TEXTURE_COUNT] = { "jupiterC.tga", "hud.tga","starField.tga","tycho_cyl_glow.tga" };

const int hoopCount = 6;
HoopWithCrystal* hoops[hoopCount];
int collectedCount = 0;

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

void readEulerAngles(EulerAngles* e)
{
	*e = *new EulerAngles(Xangle, Yangle, Zangle);
}

// Write RotationMatrix object r values to global matrixData.
void writeMatrixData(RotationMatrix r)
{
	for (int i = 0; i < 16; i++) matrixData[i] = r.getMatrixData(i);
}

bool isCollided(HoopWithCrystal hoop) {
	GLint xMin = hoop.positionX - hoop.radius;
	GLint xMax = hoop.positionX + hoop.radius;
	GLint yMin = hoop.positionY - hoop.radius;
	GLint yMax = hoop.positionY + hoop.radius;
	GLint zMin = hoop.positionZ * -1 - 32;
	GLint zMax = hoop.positionZ * -1 + 32;
	float theta = yRotation * (M_PI / 180);
	float phi = zRotation * (M_PI / 180);
	float xPosition = zMovement * sin(theta) + xMovement * -1 * cos(theta);
	float zPosition = zMovement * cos(theta) - xMovement * -1 * sin(theta);
	float yPosition = xMovement * sin(phi) + yMovement * -1 * cos(phi);
	return (xPosition >= xMin && xPosition <= xMax) &&
		(yPosition >= yMin && yPosition <= yMax) &&
		(zPosition >= zMin && zPosition <= zMax);
}


void setupHoops() {
	//for (int i = 0; i < hoopCount; i++) {
		//Random generation code copied from https://stackoverflow.com/a/7560564/9689970
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<> randspace(20, 200); // define the range

	hoops[0] = new HoopWithCrystal(40, 50, -100, 50);
	hoops[1] = new HoopWithCrystal(-40, 50, -150, 50);
	hoops[2] = new HoopWithCrystal(40, -50, -200, 50);
	hoops[3] = new HoopWithCrystal(140, 50, -250, 50);
	hoops[4] = new HoopWithCrystal(40, 150, -300, 50);
	hoops[5] = new HoopWithCrystal(100, 100, -350, 50);
	//}
	(*hoops[0]).makeNext();
}


// Called to draw scene
void RenderScene(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0); //clear the screen to black
	// Clear the window with current clearing colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glPushMatrix();

	Environment env = Environment(textures[IMAGE4]);
	Quaternion curr = Quaternion(xRotation, yRotation, zRotation);
	rotationQuaternion = identityQuaternion.multiplyBy(curr);
	RotationMatrix rm = rotationQuaternion.quaternionToRotationMatrix();
	for (int i = 0; i < 16; i++) {
		matrixData[i]  = rm.getMatrixData(i);
	}
	//glMultMatrixf(matrixData);
	glRotatef(zRotation, 0, 0, 1);
	glRotatef(yRotation, 0, 1, 0);
	glRotatef(xRotation, 1, 0, 0);
	glTranslatef(xMovement, yMovement, zMovement);

	/*GLfloat matrixMV[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixMV);*/
	//cout << matrixMV[12] << " " << matrixMV[13] << " " << matrixMV[14] << endl;

	Jupiter jupiter = Jupiter(jupiterRotation, textures[IMAGE1]);
	for (int i = 0; i < hoopCount; i++) {
		(*hoops[i]).drawHoop(textures[IMAGE3], hoopsRotation);
		if ((*hoops[i]).isHit && nextHoop == i) {
			(*hoops[i]).revertColor();
			if (i != hoopCount - 1) {
				(*hoops[i + 1]).makeNext();
			}
			(*hoops[i]).isHit = false;
			collectedCount++;
			nextHoop++;
		}
	}

	if (collectedCount == hoopCount) {
		setupHoops();
		collectedCount = 0;
		nextHoop = 0;
	}

	if (hudOn) {
		ShipHud hud = ShipHud(windowWidth, windowHeight, textures[IMAGE2]);
		hud.setDisplayInfo(collectedCount, xMovement, yMovement, zMovement, xRotation, yRotation, zRotation);
	}
	glPopMatrix();

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

	// Initialize global matrixData.
	for (int i = 0; i < 16; i++) {
		matrixData[i] = 0.0;
	}
	matrixData[0] = matrixData[5] = matrixData[10] = matrixData[15] = 1.0;
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

	if (zMovementEnd != 0) {
		zMovement += (zMovementEnd / 5);
		zMovementEnd -= (zMovementEnd / 5);
	}
	if (yMovementEnd != 0) {
		yMovement += (yMovementEnd / 5);
		yMovementEnd -= (yMovementEnd / 5);
	}
	if (xMovementEnd != 0) {
		xMovement += (xMovementEnd / 5);
		xMovementEnd -= (xMovementEnd / 5);
	}
	if (xRotationEnd != 0) {
		xRotation += (xRotationEnd / 5);
		if (xRotation >= 360) {
			xRotation = 0;
		}
		if (xRotation < 0) {
			xRotation = 360;
		}
		xRotationEnd -= (xRotationEnd / 5);
	}
	if (yRotationEnd != 0) {
		yRotation += (yRotationEnd / 5);
		if (yRotation >= 360) {
			yRotation = 0;
		}
		if (yRotation < 0) {
			yRotation = 360;
		}
		yRotationEnd -= (yRotationEnd / 5);
	}
	if (zRotationEnd != 0) {
		zRotation += (zRotationEnd/ 5);
		if (zRotation >= 360) {
			zRotation = 0;
		}
		if (zRotation < 0) {
			zRotation = 360;
		}
		zRotationEnd -= (zRotationEnd / 5);
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
		yMovementEnd -= 10.0;
		break;
	case GLUT_KEY_DOWN:
		yMovementEnd += 10.0;
		break;
	case GLUT_KEY_RIGHT:
		zRotationEnd -= 10;
		break;
	case GLUT_KEY_LEFT:
		zRotationEnd += 10;
		break;
	default:
		break;
	}
}

void keyboardFunc(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'w':
		zMovementEnd += 10;
		break;
	case 's':
		zMovementEnd -= 10;
		break;
	case 'a':
		xMovementEnd += 10;
		break;
	case 'd':
		xMovementEnd -= 10;
		break;
	case 'q':
		xRotationEnd += 10;
		break;
	case 'e':
		xRotationEnd -= 10;
		break;
	case 'z':	
		yRotationEnd += 10;
		break;
	case 'x':	
		yRotationEnd -= 10;
		break;
	case 'h':
		hudOn = !hudOn;
		break;
	case 'r':
		zRotationEnd = 0 - zRotation;
		yRotationEnd = 0 - yRotation;
		xRotationEnd = 0 - xRotation;
		break;
	default:
		break;
	}
}

void printInstructions() {
	cout << "Welcome to a 3D Space Game designed in OpenGL with GLUT" << endl;
	cout << "The aim of the game is to move your first person character" << endl;
	cout << "through space to collect the crystals withing the spanning hoops" << endl;
	cout << endl;
	cout << "When all hoops have been collected the game will reset" << endl;
	cout << endl;
	cout << "Controls: " << endl;
	cout << endl;
	cout << "Left Arrow: Roll the Ship Left" << endl;
	cout << "Right Arrow: Roll the Ship Right" << endl;
	cout << "Up Arrow: Move the ship up" << endl;
	cout << "Down Arrow: Move the ship down" << endl;
	cout << "Q: Rotate the ship backwards" << endl;
	cout << "E: Rotate the ship forwards" << endl;
	cout << "S: Move the ship backwards" << endl;
	cout << "W: Move the ship forwards" << endl;
	cout << "D: Move the ship right" << endl;
	cout << "A: Move the ship left" << endl;
	cout << "Z: Rotate the ship left" << endl;
	cout << "X: Rotatae the ship right" << endl;
	cout << "H: Turn the HUD ON/OFF" << endl;
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
	printInstructions();
	glutMainLoop();
	return 0;
}






