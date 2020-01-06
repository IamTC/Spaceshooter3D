#pragma once
#include "Quaternion.h"

class EulerAngles
{
public:
	EulerAngles() { }
	EulerAngles(float alphaVal, float betaVal, float gammaVal)
	{
		alpha = alphaVal; beta = betaVal; gamma = gammaVal;
	}
	float getAlpha() { return alpha; }
	float getBeta() { return beta; }
	float getGamma() { return gamma; }
	Quaternion eulerAnglesToQuaternion();
private:
	float alpha, beta, gamma;
};