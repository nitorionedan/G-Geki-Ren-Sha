/// ボスキャラはこのクラスを継承する
#pragma once


class BossTask
{

public:
	virtual ~BossTask(){}
	virtual void Update()	= 0;
	virtual void Draw()		= 0;
	virtual bool HitCheck(const double& ColX, const double& ColY, const int& DamagePoint) = 0;
};