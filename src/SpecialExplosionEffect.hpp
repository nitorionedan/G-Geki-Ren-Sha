#pragma once
#include "Vector2D.hpp"
#include <vector>

struct tEffect
{
	Vector2D pos;
	double range, addRange;
	double alpha, addAlpha;
	int time;
	bool isPlay;

	bool operator==(const tEffect& other) const;
};

class SpecialExplosionEffect
{


public:
	SpecialExplosionEffect();
	~SpecialExplosionEffect();
	void Draw();
	void Update();
	void Play(double x, double y);

private:
	std::vector<tEffect> m_effect;
	int sh;
	int gh;
};