#pragma once
// Rotation matrix class.
class RotationMatrix
{
public:
	RotationMatrix() { }
	RotationMatrix(float matrixDataVal[16])
	{
		for (int i = 0; i < 16; i++) matrixData[i] = matrixDataVal[i];
	}
	float getMatrixData(int i) { return matrixData[i]; }
	
private:
	float matrixData[16];
};
