#pragma once
#include "RotationMatrix.h"
//Code referenced from https://wrf.ecse.rpi.edu/wiki/ComputerGraphicsFall2013/guha/Code/
class Quaternion {
public:
	Quaternion() {}
	Quaternion(float pitch, float yaw, float roll);
	Quaternion(float wVal, float xVal, float yVal, float zVal);
	float getW() { return w; }
	float getX() { return x; }
	float getY() { return y; }
	float getZ() { return z; }
	Quaternion multiplyBy(Quaternion q2);
	Quaternion slerp(Quaternion q2, float t);
	RotationMatrix quaternionToRotationMatrix();
	void normalise();
	Quaternion getConjugate();
private:
	float w, x, y, z;
	float TOLERANCE = 0.00001f;
};