#pragma once


class ShotTask
{
public:
	ShotTask(){}
	virtual ~ShotTask(){}

	virtual void Update(const double& PosX, const double& PosY) = 0;
	virtual void Draw() = 0;
	virtual void Fire(const double& SPEED, const double& ANGLE) = 0;
	virtual void Fire(const double& PosX, const double& PosY, const double& SPEED, const double& ANGLE) = 0;
	virtual bool IsHit(const double & ColX, const double & ColY, const double & ColR) = 0;
};