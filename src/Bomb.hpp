#pragma once

#include "Vector2D.hpp"
#include <array>
#include <memory>

#define EFFECT_NUM 10


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
	bool IsHit(const int& ColCircle, const double& ColX, const double& ColY);

private:
	void DrawEffect();
	void PlayEffect();
	void MoveEffect();

	Vector2D pos;

//	std::array<BombEffect, 10>	effect;
	BombEffect effect[EFFECT_NUM];

	int time;
	int bombNum;
	int gh;
	int sh;
	int shiftLevel;
	bool isBomb;
};

/*
class Bomb
{
public:
	Bomb(){}
	~Bomb() {}
	void Update(){}
	void Draw(){}
	void SetParam(Vector2D& pos, int& shiftLevel, int& bombNum){}
	void Fire(){}
	bool IsHit(const int& ColCircle, const double& ColX, const double& ColY) { return false; }

private:
	void DrawEffect(){}
	void PlayEffect(){}
	void MoveEffect(){}

	Vector2D pos;

	//	std::array<BombEffect, 10>	effect;
	BombEffect effect[EFFECT_NUM];

	int time;
	int bombNum;
	int gh;
	int sh;
	int shiftLevel;
	bool isBomb;
};
*/

class IBomb
{
public:
	~IBomb(){}
	static void set(std::shared_ptr<Bomb> bomb);
	static void SetParam(Vector2D& pos, int& shiftLevel, int& bombNum);
	static void Fire() {
		mBomb->Fire();
	}
	static bool IsHit(const int& ColCircle, const double& ColX, const double& ColY) {
		return mBomb->IsHit(ColCircle, ColX, ColY);
	}

private:
	IBomb() { mBomb = nullptr; }

	static std::shared_ptr<Bomb> mBomb;
};