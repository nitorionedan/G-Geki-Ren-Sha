#pragma once

#include "Explosion.hpp"
#include "Counter.hpp"
#include "Vector2D.hpp"

#include <array>

#define EXPLOSION_S_NUM 30


class ExplosionS
{
public:
	ExplosionS(eEpl_tlong type_);
	~ExplosionS();
	void Update();
	void Draw();
	void Fire(const int& INDEX);
	void PlayAnime(const double& PlayX, const double& PlayY);

private:
	ExplosionS();
	void Move();

	const eEpl_tlong type;

	Explosion* explosion;
	Explosion* explosion2;
	Counter** c_play;
	std::array<Vector2D, EXPLOSION_S_NUM> pos;
	std::array<bool, EXPLOSION_S_NUM> isPlay;
	std::array<bool, EXPLOSION_S_NUM> isRed;
};