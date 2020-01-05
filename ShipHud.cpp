#include "GL/freeglut.h"
#include "ShipHud.h"

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

	ShipHud::resetPerspectiveProjection();
}

void ShipHud::setScore() {

}
/*
Converts the current projection to a 2d model.
*/
void ShipHud::setOrthographicProjection(GLint screenWidth, GLint screenHeight) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, screenWidth, screenHeight, 0.0, 0.0, 10.0);
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