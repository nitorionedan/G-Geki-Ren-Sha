#pragma once

#include "Counter.hpp"
#include "Vector2D.hpp"
#include "EffectTask.hpp"

#include <array>
#include <memory>


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


class IHitEffect 
{
public:
	~IHitEffect() {}
	static void set(std::shared_ptr<HitEffect> hitEffect);
	static void reset() {
		mHitEffect.reset();
	}
	static void PlayAnime(Vector2D& pos) {
		mHitEffect->PlayAnime(pos.x, pos.y);
	}

private:
	IHitEffect() { mHitEffect = nullptr; }
	static std::shared_ptr<HitEffect> mHitEffect;
};