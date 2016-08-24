#pragma once

#include "EffectTask.hpp"
#include "Vector2D.hpp"
#include "Counter.hpp"
#include "Explosion.hpp"
#include "ExplosionS.hpp"

#include <array>


enum eExplosion_t
{
	eExplosion_small,
	eExplosion_normal,
	eExplosion_big,

	eExplosion_long,
};


class ExplosionEffect : public EffectTask
{
private:
	static const int EXEF_ALL_NUM = 30;

public:
	ExplosionEffect(eExplosion_t kind_);
	~ExplosionEffect();
	virtual void Update() override;
	virtual void Draw() override;
	virtual void PlayAnime(const double& PlayX, const double& PlayY) override;
	void Play(const int& INDEX);

private:
	ExplosionEffect() {}
	eExplosion_t kind;
	ExplosionS* mExplosionS;
	Counter** c_play;
	std::array<Vector2D, EXEF_ALL_NUM> pos;
	std::array<bool, EXEF_ALL_NUM> isPlay;		// ”š”­‚Ì‰ò‚P‚Â
	int sh_ex;								// ”š”­‰¹
};