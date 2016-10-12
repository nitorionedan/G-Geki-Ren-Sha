#pragma once

#include "Vector2D.hpp"
#include <vector>
#include <memory>


class EneShotMove;


enum class eEneShot
{
	power,
	ball,
	longer,
	big,
	suriken,
};


enum class eEneShotMove
{
	straight,
	follow,
	accel,
	sniper,
	snipeMiss,
};


class EneShotCreater
{
private:
	static const double BD_LEFT;
	static const double BD_RIGHT;
	static const double BD_TOP;
	static const double BD_BOTTOM;

public:
	EneShotCreater();
	~EneShotCreater();
	void Update();
	void Draw();
	void Fire(eEneShot shotType, eEneShotMove moveType, Vector2D& firstPos, double& speed);
};


class IEneShotCreater
{
public:
	~IEneShotCreater(){}
	static void set(std::shared_ptr<EneShotCreater> eneShotCreater);
	static void reset() {
		mEShotCreater.reset();
	}
	static void Fire(eEneShot shotType, eEneShotMove moveType, Vector2D& firstPos, double& speed);

private:
	IEneShotCreater() { mEShotCreater = nullptr; }
	static std::shared_ptr<EneShotCreater> mEShotCreater;
};