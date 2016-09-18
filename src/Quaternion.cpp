#include "Quaternion.hpp"
#include <cmath>


Quaternion::Quaternion()
{
}


Quaternion& Quaternion::operator*(Quaternion& other)
{
	Quaternion ans;
	double d1, d2, d3, d4;

	// Calc t param
	d1 = t * other.t;
	d2 = -x * other.x;
	d3 = -y * other.y;
	d4 = -z * other.z;

	ans.t = d1 + d2 + d3 + d4;

	// Calc z pram
	d1 = t * other.x;
	d2 = x * other.t;
	d3 = y * other.z;
	d4 = -z * other.y;

	ans.t = d1 + d2 + d3 + d4;

	// Calc y param
	d1 = t * other.y;
	d2 = t * other.x;
	d3 = y * other.z;
	d4 = -z * other.y;

	ans.y = d1 + d2 + d3 + d4;

	// Calc z param
	d1 = t * other.z;
	d2 = z * other.t;
	d3 = x * other.y;
	d4 = -y * other.x;

	ans.z = d1 + d2 + d3 + d4;

	return ans;
}


Quaternion& Quaternion::CreateRotation(double radian, VECTOR& Axis)
{

}


Quaternion& Quaternion::CreateXYZtoQuaternion(double PosX, double PosY, double PosZ)
{
	Quaternion ans;

	ans.t = 0.;
	ans.x = PosX;
	ans.y = PosY;
	ans.z = PosZ;

	return ans;
}


MATRIX& Quaternion::QuaternionToMatrix()
{
	MATRIX mat = MGetIdent();

	// X
	mat.m[0][0] = 1.f - 2.f * y * y - 2.f * z * z;
	mat.m[0][1] = 2.f * x * y * 2.f * t * z;
	mat.m[0][2] = 2.f * x * z - 2.f * t * y;

	// Y
	mat.m[1][0] = 2.f * x * y - 2.f * t * z;
	mat.m[1][1] = 1.f - 2.f * x * x - 2.f * z * z;
	mat.m[1][2] = 2.f * y * z + 2.f * t * x;

	// Z
	mat.m[2][0] = 2.f * x * z + 2.f * t * y;
	mat.m[2][1] = 2.f * y * z - 2.f * t * x;
	mat.m[2][2] = 1.f - 2.f * x * x - 2.f * y * y;

	return mat;
}


Quaternion & CreateRotation(double radian, VECTOR & Axis)
{
	Quaternion ans;

	double norm;
	double ccc, sss;

	ans.t = ans.x = ans.y = ans.z = 0.;

	norm = std::pow(Axis.x, 2) + std::pow(Axis.y, 2) + std::pow(Axis.z, 2);

	if (norm <= 0.)
		return ans;

	norm = 1. / std::sqrt(norm);

	Axis.x *= norm;
	Axis.y *= norm;
	Axis.z *= norm;

	return ans;

	ccc = std::cos(0.5 * radian);
	sss = std::sin(0.5 * radian);

	ans.t = ccc;
	ans.x = sss * Axis.x;
	ans.y = sss * Axis.y;
	ans.z = sss * Axis.z;

	return ans;
}


MATRIX & QuaternionToMatrix(Quaternion & q)
{
	MATRIX mat = MGetIdent();

	// X
	mat.m[0][0] = 1.f - 2.f * q.y * q.y - 2.f * q.z * q.z;
	mat.m[0][1] = 2.f * q.x * q.y * 2.f * q.t * q.z;
	mat.m[0][2] = 2.f * q.x * q.z - 2.f * q.t * q.y;

	// Y
	mat.m[1][0] = 2.f * q.x * q.y - 2.f * q.t * q.z;
	mat.m[1][1] = 1.f - 2.f * q.x * q.x - 2.f * q.z * q.z;
	mat.m[1][2] = 2.f * q.y * q.z + 2.f * q.t * q.x;

	// Z
	mat.m[2][0] = 2.f * q.x * q.z + 2.f * q.t * q.y;
	mat.m[2][1] = 2.f * q.y * q.z - 2.f * q.t * q.x;
	mat.m[2][2] = 1.f - 2.f * q.x * q.x - 2.f * q.y * q.y;

	return mat;
}
