#pragma once

#include "ShotTask.hpp"
#include "Player.hpp"


class Shot
{
public:
	static const double SC_LIMIT_XL;
	static const double SC_LIMIT_XR;
	static const double SC_LIMIT_YT;
	static const double SC_LIMIT_YB;

	Shot(ShotTask* shot);
	~Shot();
	void Update(const double& PosX, const double& PosY) {
		mShot->Update(PosX, PosY);
	}
	void Draw() {
		mShot->Draw();
	}
	void Fire(const double& SPEED, const double& ANGLE) {
		mShot->Fire(SPEED, ANGLE);
	}
	void Fire(const double& PosX, const double& PosY, const double& SPEED, const double& ANGLE) {
		mShot->Fire(PosX, PosY, SPEED, ANGLE);
	}
	bool IsHit(const double & ColX, const double & ColY, const double & ColR) {
		return mShot->IsHit(ColX, ColY, ColR);
	}

private:
	Shot::Shot() {}
	ShotTask* mShot;
};