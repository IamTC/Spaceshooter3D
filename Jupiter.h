#pragma once
#include "GL/freeglut.h"
class Jupiter {
public: Jupiter(GLfloat rotation, GLuint texture);
private:
	GLfloat  whiteLightBright[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat  redLight[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat  whiteLightLessBright[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat	 lightPos[4] = { 0, 0, 0, 0};
	static GLUquadric* sphere;
	void setupLighting();
	void renderPlanet(GLfloat rotation, GLuint texture);
	void disableLighting();
};