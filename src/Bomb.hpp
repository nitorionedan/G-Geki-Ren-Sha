#pragma once

#include "Vector2D.hpp"
#include <array>
#include <memory>


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
	~Bomb(){}
	void Update();
	void Draw();
	void SetParam(Vector2D& pos, int& shiftLevel, int& bombNum);
	void Fire();
	static bool IsHit(const int& ColCircle, const double& ColX, const double& ColY);

private:
	void DrawEffect();
	void PlayEffect();
	void MoveEffect();

	static Vector2D pos;

//	std::array<BombEffect, 10>	effect;
	static BombEffect effect[];

	Vector2D playerPos;
	int time;
	int bombNum;
	int gh;
	int sh;
	int shiftLevel;
	bool isBomb;
};