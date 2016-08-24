#pragma once

#include "Task.hpp"
#include "Counter.hpp"
#include "Vector2D.hpp"
#include "EffectTask.hpp"

#include <array>


class HitEffect : public EffectTask
{
public:
	HitEffect();
	~HitEffect();
	virtual void Update() override;
	virtual void Draw() override;
	void PlayAnime(const double& MyX, const double& MyY);

private:
	std::array<Counter*, 100> c_hit;
	std::array<Vector2D, 100> pos;
	std::array<bool, 100> isHit;
	std::array<double, 100> angle;
	std::array<double, 100> exrate;
	int gh_hit[4];
};