/// ボスキャラはこのクラスを継承する
#pragma once
#include  <DxLib.h>
#include "Player.hpp"


static COLOR_F const CyanF = GetColorF(0.f, 1.f, 1.f, 1.f);
static COLOR_F const RedF = GetColorF(1.f, 0.f, 0.f, 1.f);
static COLOR_F const YellowF = GetColorF(1.f, 1.f, 0.f, 1.f);


class BossTask
{
public:
	virtual ~BossTask(){}
	virtual void Update() = 0;
	virtual void Draw()		= 0;
	virtual const Vector2D& GetPos() const = 0;
	virtual bool HitCheck(const double& ColX, const double& ColY, const int& DamagePoint) = 0;
	virtual const bool IsDead() const = 0;
};