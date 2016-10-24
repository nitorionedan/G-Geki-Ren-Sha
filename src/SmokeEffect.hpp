#pragma once
#include "Vector2D.hpp"
#include <vector>

enum class eSmokeColor
{
	green,
};

class tSmoke
{
public:
	Vector2D pos;
	eSmokeColor colorType;
	double rad;
	int time;
	bool isExist;

	bool operator==(const tSmoke& other) const {
		return (rad == other.rad) &&
			(time == other.time) &&
			(isExist == other.isExist) &&
			(pos == other.pos);
	}
};

class SmokeEffect
{
public:
	SmokeEffect();
	~SmokeEffect();
	void Update();
	void Draw();
	void Update_Time();
	void CheckEndTime();
	void PlayAnime(const double& x, const double& y, eSmokeColor colorType);

private:
	const int All_time;

	std::vector<tSmoke> m_smoke;
	int gh_green[8];
};