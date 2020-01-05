#pragma once
#include "GL/freeglut.h"
class HoopWithCrystal {
public :
	bool isHit;
	HoopWithCrystal(GLint pX, GLint pY, GLint pZ, GLint r);
	void drawHoop(GLuint texture, GLfloat rotation);
	void makeNext();
	GLint positionX;
	GLint positionY;
	GLint positionZ;
	GLint radius;
	GLfloat crystalColorRed;
	GLfloat crystalColorBlue;
	GLfloat crystalColorGreen;
private:
};