#pragma once
#include <DxLib.h>


class Quaternion
{
public:
	Quaternion();
	~Quaternion(){}
	double t, x, y, z; // real, x, y, z

private:
	// Calc quaternion
	Quaternion& operator*(Quaternion& other);

	// Rotation quaternion
	static Quaternion& CreateRotation(double radian, VECTOR& Axis);

	// Position quaternoon
	static Quaternion& CreateXYZtoQuaternion(double PosX, double PosY, double PosZ);

	// Quarteniojn to Rotation matrix
	MATRIX& QuaternionToMatrix();
};


// Rotation quaternion
Quaternion& CreateRotation(double radian, VECTOR& Axis);

// Quarteniojn to Rotation matrix
MATRIX& QuaternionToMatrix(Quaternion& q);
