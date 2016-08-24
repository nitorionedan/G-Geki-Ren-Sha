#pragma once

#include "Vector2D.hpp"

#include <array>


class BombEffect
{
public:
	BombEffect() {}
	~BombEffect() {}
	int rad;
	bool isExist;
};


class Bomb
{
public:
	Bomb();
	~Bomb();
	void Update();
	void Draw();
	void Fire(int shiftLevel);
	static bool IsHit(const int& ColCircle, const double& ColX, const double& ColY);

private:
	void DrawEffect();
	void PlayEffect();
	void MoveEffect();

//	std::array<BombEffect, 10>	effect;
	static BombEffect effect[];

	int time;

	// ‰æ‘œ
	int gh;

	// Œø‰Ê‰¹
	int sh;

	bool isBomb;
};