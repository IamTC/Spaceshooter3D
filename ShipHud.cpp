#include "GL/freeglut.h"
#include "ShipHud.h"
#include <string>

using namespace std;
/*
Draws the Heads up display. The given texture will be stretched over the screen.
*/
ShipHud::ShipHud(GLint screenWidth, GLint screenHeight, GLuint texture) {

	ShipHud::setOrthographicProjection(screenWidth, screenHeight);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glClear(GL_DEPTH_BUFFER_BIT);
	//glDisable(GL_BLENDING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
	glColor3f(0.8, 0.8, 0.8);
	//draw whatever is using the alpha channel
	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 1.0);
	glVertex2f(0, 0);
	glTexCoord2f(1.0, 1.0);
	glVertex2f(screenWidth, 0);
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex2f(screenWidth, screenHeight);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(0, screenHeight);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void ShipHud::setDisplayInfo(int score, GLfloat px, GLfloat py, GLfloat pz, GLfloat rx, GLfloat ry, GLfloat rz) {
	
	string numStr = "Crystals Collected: ";
	numStr.append(to_string(score));
	char charArr[50];
	strcpy_s(charArr, numStr.c_str());
	
	glColor3f(0, 1, 0.0);
	glPushMatrix();
	glTranslatef(10, 50, 1);
	glScalef(0.3, -0.3, 1);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)charArr);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex2f(30,70);
	glVertex2f(300, 70);
	glEnd();
	glFlush();
	glPopMatrix();

	numStr = "Ship position X: "; // Convers the score to a char array
	numStr.append(to_string((int)px));
	strcpy_s(charArr, numStr.c_str());

	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslatef(10, 100, 1);
	glScalef(0.15, -0.15, 1);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)charArr);
	glPopMatrix();

	numStr = "Ship position Y: "; // Convers the score to a char array
	numStr.append(to_string((int)py));
	strcpy_s(charArr, numStr.c_str());

	glColor3f(0, 1, 0);
	glPushMatrix();
	glTranslatef(10, 150, 1);
	glScalef(0.15, -0.15, 1);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)charArr);
	glPopMatrix();

	numStr = "Ship position Z: "; // Convers the score to a char array
	numStr.append(to_string((int)pz));
	strcpy_s(charArr, numStr.c_str());

	glColor3f(0, 0, 1);
	glPushMatrix();
	glTranslatef(10, 200, 1);
	glScalef(0.15, -0.15, 1);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)charArr);
	glPopMatrix();

	glColor3f(1, 1, 1);
	numStr = "Rotation X: "; // Convers the score to a char array
	numStr.append(to_string((int)rx));
	strcpy_s(charArr, numStr.c_str());

	glPushMatrix();
	glTranslatef(10, 250, 1);
	glScalef(0.12, -0.12, 1);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)charArr);
	glPopMatrix();

	numStr = "Rotation Y: "; // Convers the score to a char array
	numStr.append(to_string((int)ry));
	strcpy_s(charArr, numStr.c_str());

	glPushMatrix();
	glTranslatef(10, 280, 1);
	glScalef(0.12, -0.12, 1);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)charArr);
	glPopMatrix();

	numStr = "Rotation Z: "; // Convers the score to a char array
	numStr.append(to_string((int)rz));
	strcpy_s(charArr, numStr.c_str());

	glPushMatrix();
	glTranslatef(10, 310, 1);
	glScalef(0.12, -0.12, 1);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)charArr);
	glPopMatrix();
	
	ShipHud::resetPerspectiveProjection();

}
/*
Converts the current projection to a 2d model.
*/
void ShipHud::setOrthographicProjection(GLint screenWidth, GLint screenHeight) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, screenWidth, screenHeight, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_CULL_FACE);

	glClear(GL_DEPTH_BUFFER_BIT);
}

//Resets the projection mode to perspective.
void ShipHud::resetPerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}