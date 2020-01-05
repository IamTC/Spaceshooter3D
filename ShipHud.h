#pragma once
class ShipHud {
public:
	ShipHud(GLint screenWidth, GLint screenHeight, GLuint texture);

private:
	void setOrthographicProjection(GLint screenWidth, GLint screenHeight);
	void resetPerspectiveProjection();
	void setScore();
};