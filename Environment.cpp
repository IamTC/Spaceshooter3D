#include "GL/freeglut.h"
#include "Environment.h"

Environment::Environment(GLuint texture) {
	Environment::setupLighting();
	Environment::drawEnvironment(texture);
	Environment::disableLighting();
}

void Environment::setupLighting() {
	GLfloat  whiteLightBright[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat  redLight[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat  whiteLightLessBright[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat	 lightPos[4] = { -200, 60, -500, 0 };

	glEnable(GL_DEPTH_TEST);	// Hidden surface removal    
	glFrontFace(GL_CCW);// Counter clock-wise polygons face out
	//glEnable(GL_CULL_FACE);		// Do not calculate inside

	//glCullFace(GL_FRONT_AND_BACK);
	glCullFace(GL_BACK);

	//// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_POINT_SMOOTH);
	////// Setup and enable light 0
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLightLessBright);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, redLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);
}
void Environment::drawEnvironment(GLuint texture) {
	//// Enable colour tracking
	glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Black blue background
	//glClearColor(0.0f, 0.0f, 0.03f, 1.0f);
	//add some lighting normals for each vertex
	//draw the texture on the front
	glEnable(GL_TEXTURE_2D);
	// glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
	glColor3f(0.8, 0.8, 0.8);

	glPushMatrix();
	//rotate the quad slightly
	glRotatef(270, 1.0, 0.0, 0.0);

	//glColor3f(0.0, 1.0, 0.0);
	gluQuadricDrawStyle(sphere, GLU_FILL);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, texture);
	gluQuadricTexture(sphere, GL_TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluSphere(sphere, 1000, 32, 32);

	glPopMatrix();
}

void Environment::disableLighting() {
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glDisable(GL_LIGHTING);
	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_LIGHT0);

	glDisable(GL_COLOR_MATERIAL);
}

GLUquadric* Environment::sphere = gluNewQuadric();