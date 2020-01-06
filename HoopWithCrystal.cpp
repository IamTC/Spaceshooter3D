#include "GL/freeglut.h"
#include "HoopWithCrystal.h"
HoopWithCrystal::HoopWithCrystal(GLint pX, GLint pY, GLint pZ, GLint r)
:isHit(false),
crystalColorRed(1.0),
crystalColorGreen(1.0),
crystalColorBlue(0.0),
positionX(pX),
positionY(pY),
positionZ(pZ),
radius(r)
{
}
void HoopWithCrystal::drawHoop(GLuint texture, GLfloat rotation) {
	//// Enable colour tracking
	//glEnable(GL_COLOR_MATERIAL);
	// Set Material properties to follow glColor values
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	glTranslatef(positionX, positionY, positionZ);
	glScalef(10, 10, 10);
	glRotatef(rotation, 0, 1, 0);
	glColor3f(crystalColorRed, crystalColorGreen, crystalColorBlue);
	glutSolidIcosahedron();
	glPopMatrix();

	// Black blue background
	//glClearColor(0.0f, 0.0f, 0.03f, 1.0f);
	//add some lighting normals for each vertex
	//draw the texture on the front
	glEnable(GL_TEXTURE_2D);
	//glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glColor3f(1, 0, 1);
	glPushMatrix();
	//rotate the quad slightly	
	glTranslatef(positionX, positionY, positionZ);
	glRotatef(-rotation, 0, 1, 0);
	glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, texture);
	//bind the texture 
	glutSolidTorus(5, radius, 8, 32);
	glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glDisable(GL_TEXTURE_GEN_T);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

}

void HoopWithCrystal::makeNext() {
	isHit = false;
	crystalColorRed = 0.0;
	crystalColorGreen = 1.0;
	crystalColorBlue = 0.0;
}

void HoopWithCrystal::revertColor() {
	crystalColorRed = 1.0;
	crystalColorGreen = 1.0;
	crystalColorBlue = 0.0;
}