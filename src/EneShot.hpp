/*
@file   EneShot class
@brief  Enemy's shot creater
@author Shohei
*/
#pragma once
#include "Vector2D.hpp"
#include <memory>
#include <vector>

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


struct tShot
{
	eShotType shotType;
	Vector2D pos, force;
	double accel;
	double rad;
	double hitRange;
	int life;
	int time;

	bool operator==(const tShot& other) {
		const bool& isSame =
			this->shotType != other.shotType ||
			this->force    != other.force ||
			this->pos      != other.pos ||
			this->accel    != other.accel ||
			this->rad      != other.rad ||
			this->hitRange != other.hitRange ||
			this->life     != other.life ||
			this->time     != other.time;
		
		return isSame;
	}
};


class EneShot
{
public:
	EneShot();
	~EneShot();
	void Update();
	void Draw();
	void Fire(eShotType type, Vector2D& pos, Vector2D& force, double accel, int life);

	// @brief       Fire in use a radian angle.
	// @param[in]   ...
	void Fire_Ang(eShotType type, Vector2D& pos, double force, double angle, double accel, int life);

private:
	void HitCheck();

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
	static void Fire(eShotType type, Vector2D pos, Vector2D force, double accel, int life) {
		mEneShot->Fire(type, pos, force, accel, life);
	}
	static void Fire_Ang(eShotType type, Vector2D pos, double force, double angle, double accel, int life) {
		mEneShot->Fire_Ang(type, pos, force, angle, accel, life);
	}

private:
	IEneShot() { mEneShot = nullptr; }
	static std::shared_ptr<EneShot> mEneShot;
};