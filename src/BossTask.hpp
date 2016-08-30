/// ボスキャラはこのクラスを継承する
#pragma once
#include "Player.hpp"


class BossTask
{

public:
	virtual ~BossTask(){}
	virtual void Update(const Player& player) = 0;
	virtual void Draw()		= 0;
	virtual bool HitCheck(const double& ColX, const double& ColY, const int& DamagePoint) = 0;
};