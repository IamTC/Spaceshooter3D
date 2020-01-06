#pragma once
class ShipHud {
public:
	ShipHud(GLint screenWidth, GLint screenHeight, GLuint texture);
	void setDisplayInfo(int score, GLfloat px, GLfloat py, GLfloat pz, GLfloat rx, GLfloat ry, GLfloat rz);
private:
	void setOrthographicProjection(GLint screenWidth, GLint screenHeight);
	void resetPerspectiveProjection();
};