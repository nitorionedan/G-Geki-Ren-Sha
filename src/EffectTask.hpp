#pragma once


class EffectTask
{
public:
	virtual ~EffectTask(){}
	virtual void Update() = 0;
	virtual void Update(const double& X, const double& Y){}
	virtual void Draw() = 0;
	virtual void PlayAnime(const double& X, const double& Y) = 0;
};