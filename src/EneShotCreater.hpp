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
};


class EneShotCreater
{
	class EneShot
	{
	public:
		explicit EneShot(eEneShot shotType, EneShotMove* moveType, Vector2D firstPos);
		~EneShot();
		void Update(EneShotCreater* pEneShotCreater);
		void Draw(const int& GrHandle);

		EneShotMove* mMove;
	private:
		EneShot();
		void Initialize();

		Vector2D pos;
		eEneShot shotType;
	};

public:
	EneShotCreater();
	~EneShotCreater();
	void Update();
	void Draw();

private:
	void Initialize();
	void Move();
	void CheckCollider();
	void Fire(eEneShot shotType, eEneShotMove moveType, Vector2D& firstPos);

	std::vector<std::unique_ptr<EneShot>> mEneShot;
	int gh_power[24],
		gh_ball[14],
		gh_long[5],
		gh_big[4],
		gh_suriken[10];
};


class IEneShotCreater
{
};