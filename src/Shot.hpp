#pragma once

#include "ShotTask.hpp"


class Shot
{
public:
	static const double SC_LIMIT_XL;
	static const double SC_LIMIT_XR;
	static const double SC_LIMIT_YT;
	static const double SC_LIMIT_YB;

	Shot(ShotTask* shot);
	~Shot();
	void Update(const double& PosX, const double& PosY);
	void Draw();
	void Fire(const double& SPEED, const double& ANGLE);
	void Fire(const double& PosX, const double& PosY, const double& SPEED, const double& ANGLE);
	bool IsHit(const double & ColX, const double & ColY, const double & ColR);

private:
	Shot::Shot() {}
	ShotTask* mShot;
};