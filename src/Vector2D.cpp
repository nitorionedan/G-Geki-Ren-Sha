#include "Vector2D.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <DxLib.h>

#undef min
#undef max


Vector2D const Vector2D::ZERO(0., 0.);
Vector2D const Vector2D::LEFT(-1., 0.);
Vector2D const Vector2D::RIGHT(1., 0.);
Vector2D const Vector2D::UP(0., 1);
Vector2D const Vector2D::DOWN(0., -1);


Vector2D::Vector2D(double x, double y)
{
	this->x = x;
	this->y = y;
}


void Vector2D::SetZero()
{
	x = 0.;
	y = 0.;
}


void Vector2D::BorderStop(double* pos, const double MIN, const double MAX)
{
	assert(MAX > MIN && "Vector2D::BorderStop()");

	*pos = std::min(MAX, std::max(MIN, *pos));
}


void Vector2D::AddVecAngele(double * PosX, double * PosY, const double& angle, const double& force)
{
	*PosX += std::cos(angle) * force;
	*PosY += std::sin(angle) * force;
}


bool Vector2D::CirclePointCollision(const double& MyX, const double& MyY, const double& TarX, const double& TarY, const double& Radius)
{
	const double& WIDTH = (TarX - MyX) * (TarX - MyX);
	const double& HEIGHT = (TarY - MyY) * (TarY - MyY);
	const double& DISTANCE = (WIDTH + HEIGHT);
	const double& RADIUS = Radius * Radius;
	const bool& isHit = (RADIUS >= DISTANCE);
	
	return isHit;
}


bool Vector2D::CirclesCollision(const double & Range1, const double & Range2, const double & X1, const double & Y1, const double & X2, const double & Y2)
{
	const double& hLengrth = (Range1 + Range2);
	const double& xLength = (X1 - X2);
	const double& yLength = (Y1 - Y2);
	const bool& isHit = (hLengrth * hLengrth >= xLength * xLength + yLength * yLength);

	return isHit;
}


bool Vector2D::RectanglePointCollision(const double & MyX, const double & MyY, const double & Width, const double & Height, const double & TarX, const double & TarY)
{
	const double& MyX2 = (MyX + Width);
	const double& MyY2 = (MyY + Height);
	const bool& isHit = (MyX <= TarX && TarX <= MyX2 && MyY <= TarY && TarY <= MyY2);

	return isHit;
}


const Vector2D & Vector2D::Normalize() const
{
	const double& len = Length();
	if (len < DBL_EPSILON)
		return Vector2D::ZERO;
	return *this / len;
}


const double Vector2D::Length() const {
	return std::sqrt(DotProduct(*this));
}

const double Vector2D::ToRad() const {
	return std::atan2(x, y);
}


const double Vector2D::ToAngle() const {
	return std::atan2(x, y) * 180 / DX_PI;
}
