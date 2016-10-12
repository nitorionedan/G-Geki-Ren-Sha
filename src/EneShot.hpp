#pragma once
#include "Vector2D.hpp"
#include <memory>
#include <vector>


enum class eShotType
{
	normal,
	star,
	wave,
	big_O,
	laser,
	longer,
};


class tShot
{
public:
	eShotType shotType;
	Vector2D pos, force;
	double accel;
	double rad;
	int life;
	int time;

	bool operator==(const tShot& other) {
		return this->shotType != other.shotType || this->force != other.force || this->pos != other.pos || this->accel != other.accel || this->time != other.time || this->life != other.life;
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

private:
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

private:
	IEneShot() { mEneShot = nullptr; }
	static std::shared_ptr<EneShot> mEneShot;
};