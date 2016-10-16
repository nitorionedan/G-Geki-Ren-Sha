/*
@file   EneShot class
@brief  Enemy's shot creater
@author Shohei
*/
#pragma once
#include "Vector2D.hpp"
#include <memory>
#include <vector>

class EneShotAI;

enum class eShotType
{
	normal,  // small blue ball
	star,    // like a star
	wave,    // power shot
	big_O,   // bossA's shot
	laser,   
	longer,  // long shot
	missile,
};

enum class eShotAI
{
	straight,
	outsideCurve,
	insideCurve,
	wave,
};

class tShot
{
public:
	tShot(){}
	explicit tShot(EneShotAI* ai);
	~tShot(){}

	eShotType shotType;
	Vector2D pos, force;
	EneShotAI* mAI;
	double accel;
	double rad;
	double rotate;
	double hitRange;
	int life;
	int time;

	bool operator==(const tShot& other)
	{
		const bool& isDifferent =
			this->shotType != other.shotType ||
			this->force != other.force ||
			this->pos != other.pos ||
			this->accel != other.accel ||
			this->rad != other.rad ||
			this->rotate != other.rotate ||
			this->hitRange != other.hitRange ||
			this->life != other.life ||
			this->time != other.time;
		return !isDifferent;
	}
};


class EneShot
{
public:
	EneShot();
	~EneShot();
	void Update();
	void Draw();
	void Fire(eShotType type, Vector2D& pos, double rotate, Vector2D& force, double accel, int life, eShotAI aiType);

	// @brief       Fire in use a radian angle.
	// @param[in]   ...
	void Fire_Ang(eShotType type, Vector2D& pos, double rotate, double force, double angle, double accel, int life, eShotAI aiType);

private:
	void HitCheck();
	void SetRange(eShotType type, double& hitRange) const;

	std::vector<tShot> shot;
	int gh_wave[24],
		gh_normal[12],
		gh_long[5],
		gh_big_O[4],
		gh_star[10];
};


class IEneShot
{
public:
	~IEneShot(){}
	static void set(std::shared_ptr<EneShot> eneShot);
	static void reset();
	/*
	@attention accel‚Í“™‘¬’¼ü‚Å‚P‚ð“ü‚ê‚Ä‚­‚¾‚³‚¢
	*/
	static void Fire(eShotType type, Vector2D& pos, double rotate, Vector2D& force, double accel, int life, eShotAI aiType = eShotAI::straight) {
		mEneShot->Fire(type, pos, rotate, force, accel, life, aiType);
	}
	/*
	@attention accel‚Í“™‘¬’¼ü‚Å‚P‚ð“ü‚ê‚Ä‚­‚¾‚³‚¢
	*/
	static void Fire_Ang(eShotType type, Vector2D& pos, double rotate, double force, double angle, double accel, int life, eShotAI aiType = eShotAI::straight) {
		mEneShot->Fire_Ang(type, pos, rotate, force, angle, accel, life, aiType);
	}

private:
	IEneShot() { mEneShot = nullptr; }
	static std::shared_ptr<EneShot> mEneShot;
};