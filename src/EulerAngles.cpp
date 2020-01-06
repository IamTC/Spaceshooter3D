#include "EulerAngles.h"
#include "Quaternion.h"
#include <cmath>
#define PI 3.14159265

Quaternion EulerAngles::eulerAnglesToQuaternion() {
	float alpha, beta, gamma;
	Quaternion* q1, * q2, * q3;

	alpha = getAlpha(); beta = getBeta(); gamma = getGamma();

	q1 = new Quaternion(cos((PI / 180.0) * (alpha / 2.0)), sin((PI / 180.0) * (alpha / 2.0)), 0.0, 0.0);
	q2 = new Quaternion(cos((PI / 180.0) * (beta / 2.0)), 0.0, sin((PI / 180.0) * (beta / 2.0)), 0.0);
	q3 = new Quaternion(cos((PI / 180.0) * (gamma / 2.0)), 0.0, 0.0, sin((PI / 180.0) * (gamma / 2.0)));

	return q1->multiplyBy(q2->multiplyBy(*q3));
}