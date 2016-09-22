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
public:
	class EneShot
	{
	public:
		EneShot(eEneShot shotType, eEneShotMove moveType, Vector2D firstPos, double speed);
		~EneShot();
		void Update(EneShotCreater* pEneShotCreater);
		void Draw(EneShotCreater* pEneShotCreater);
		bool HitCheck();

		EneShotMove* mMove;
		Vector2D pos;
		double speed, angle;

	private:
		EneShot(){}
		void Initialize();

		eEneShot shotType;
		int elapsedTime;
		double hitRange;
	};

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

private:
	std::vector<std::shared_ptr<EneShot>> mEneShot;
	int gh_power[24],
		gh_ball[12],
		gh_long[5],
		gh_big[4],
		gh_suriken[10];
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