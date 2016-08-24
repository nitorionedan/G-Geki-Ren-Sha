#pragma once

#include "Vector2D.hpp"
#include "Counter.hpp"

#include <array>

#define COUNTER_NUM 200


enum eEpl_tlong
{
	eEpl_short,
	eEpl_normal,
	eEpl_long,
};


class Explosion
{
public:
	Explosion(eEpl_tlong tlong, bool isRed);
	~Explosion();
	void Update();
	void Draw();
	void DrawTest();
	void PlayAnime(const double& PlayX, const double& PlayY, const double& SpeedX, const double& SpeedY);

	std::array<Vector2D, COUNTER_NUM> pos;

private:
	Explosion(){}

	eEpl_tlong kind;
	int gh_explo[8];
	int maxCount;
	Counter** c_play;
	std::array<Vector2D, COUNTER_NUM> speed;
	std::array<double, COUNTER_NUM> angle;
	std::array<bool, COUNTER_NUM> isPlay;
};