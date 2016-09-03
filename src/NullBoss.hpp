#pragma once
#include "BossTask.hpp"

class NullBoss : public BossTask
{
public:
	NullBoss() {}
	~NullBoss() {}
	virtual void Update() override {}
	virtual void Draw() override {}
	virtual const Vector2D& GetPos() const override { return Vector2D::ZERO; }
	virtual bool HitCheck(const double& X, const double& Y, const int& point) override { return false; }
	virtual const bool IsDead() const override { return false; }
};