#pragma once
#include "BossTask.hpp"

class NullBoss : public BossTask
{
public:
	void Update() override {}
	void Draw() override {}
	bool HitCheck(const double& X, const double& Y, const int& point) override { return false; }
};