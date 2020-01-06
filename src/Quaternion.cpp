#include "Quaternion.h"
#include <cmath>
#define PI 3.14159265
Quaternion::Quaternion(float pitch, float yaw, float roll) {

	float p = pitch * PI/180 / 2.0; 
	float y = yaw * PI / 180 / 2.0; 
	float r = roll * PI / 180 / 2.0;

	float sinp = sin(p); 
	float siny = sin(y); 
	float sinr = sin(r); 
	float cosp = cos(p); 
	float cosy = cos(y); 
	float cosr = cos(r);

	x = sinr * cosp * cosy - cosr * sinp * siny; 
	y = cosr * sinp * cosy + sinr * cosp * siny; 
	z = cosr * cosp * siny - sinr * sinp * cosy; 
	w = cosr * cosp * cosy + sinr * sinp * siny;

	normalise();
}

Quaternion::Quaternion(float wVal, float pitch, float yaw, float roll) {
	x = pitch;
	y = yaw;
	z = roll;
	w = wVal;
}

Quaternion Quaternion::multiplyBy(Quaternion q2) {
	float w1, x1, y1, z1, w2, x2, y2, z2, w3, x3, y3, z3;

	w1 = getW(); x1 = getX(); y1 = getY(); z1 = getZ();
	w2 = q2.getW(); x2 = q2.getX(); y2 = q2.getY(); z2 = q2.getZ();

	w3 = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2;
	x3 = w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2;
	y3 = w1 * y2 + y1 * w2 + z1 * x2 - x1 * z2;
	z3 = w1 * z2 + z1 * w2 + x1 * y2 - y1 * x2;

	return *new Quaternion(w3, x3, y3, z3);
}

// Spherical linear interpolation between unit quaternions q1 and q2 with interpolation parameter t.
Quaternion Quaternion::slerp(Quaternion q2, float t) {
	float w1, x1, y1, z1, w2, x2, y2, z2, w3, x3, y3, z3;
	Quaternion q2New;
	float theta, mult1, mult2;

	w1 = getW(); x1 = getX(); y1 = getY(); z1 = getZ();
	w2 = q2.getW(); x2 = q2.getX(); y2 = q2.getY(); z2 = q2.getZ();

	// Reverse the sign of q2 if q1.q2 < 0.
	if (w1 * w2 + x1 * x2 + y1 * y2 + z1 * z2 < 0)
	{
		w2 = -w2; x2 = -x2; y2 = -y2; z2 = -z2;
	}

	theta = acos(w1 * w2 + x1 * x2 + y1 * y2 + z1 * z2);

	if (theta > 0.000001)
	{
		mult1 = sin((1 - t) * theta) / sin(theta);
		mult2 = sin(t * theta) / sin(theta);
	}

	// To avoid division by 0 and by very small numbers the approximation of sin(angle)
	// by angle is used when theta is small (0.000001 is chosen arbitrarily).
	else
	{
		mult1 = 1 - t;
		mult2 = t;
	}

	w3 = mult1 * w1 + mult2 * w2;
	x3 = mult1 * x1 + mult2 * x2;
	y3 = mult1 * y1 + mult2 * y2;
	z3 = mult1 * z1 + mult2 * z2;

	return *new Quaternion(w3, x3, y3, z3);
}

RotationMatrix Quaternion::quaternionToRotationMatrix() {
	float w, x, y, z;
	float m[16];

	w = getW(); x = getX(); y = getY(); z = getZ();

	m[0] = w * w + x * x - y * y - z * z;
	m[1] = 2.0 * x * y + 2.0 * w * z;
	m[2] = 2.0 * x * z - 2.0 * y * w;
	m[3] = 0.0;
	m[4] = 2.0 * x * y - 2.0 * w * z;
	m[5] = w * w - x * x + y * y - z * z;
	m[6] = 2.0 * y * z + 2.0 * w * x;
	m[7] = 0.0;
	m[8] = 2.0 * x * z + 2.0 * w * y;
	m[9] = 2.0 * y * z - 2.0 * w * x;
	m[10] = w * w - x * x - y * y + z * z;
	m[11] = 0.0;
	m[12] = 0.0;
	m[13] = 0.0;
	m[14] = 0.0;
	m[15] = 1.0;

	return *new RotationMatrix(m);
}

void Quaternion::normalise() { 
	// Don't normalize if we don't have to 
	float  mag2 = w * w + x * x + y * y + z * z;
	if (fabs(mag2 - 1.0f) > Quaternion::TOLERANCE)
	{ 
		float mag = sqrt(mag2); 
		w /= mag; 
		x /= mag; 
		y /= mag; 
		z /= mag; 
	} 
}

Quaternion Quaternion::getConjugate() { 
	return Quaternion(w, -x, -y, -z); 
}