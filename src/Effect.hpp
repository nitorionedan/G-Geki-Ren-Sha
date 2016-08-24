#pragma once

#include "EffectTask.hpp"


class Effect : public EffectTask
{
public:
	Effect(EffectTask* EFtask);
	~Effect();
	void Update() override;
	void Update(const double& X, const double& Y);
	void Draw() override;
	void PlayAnime(const double& MyX, const double& MyY) override;

private:
	Effect() {}
	EffectTask* mEFTask;
};