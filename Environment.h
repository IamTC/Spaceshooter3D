#pragma once
#include "GL/freeglut.h"

class Environment {
public:
	Environment(GLuint texture);
private:
	void setupLighting();
	void drawEnvironment(GLuint texture);
	void disableLighting();
	static GLUquadric* sphere;
};