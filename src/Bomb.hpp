#pragma once

#include "Vector2D.hpp"
#include "Player.hpp"
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
	void Update(const Player& player);
	void Draw();
	void Fire(int shiftLevel);
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

	// ‰æ‘œ
	int gh;

	// Œø‰Ê‰¹
	int sh;

	bool isBomb;
};