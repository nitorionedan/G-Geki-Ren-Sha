#pragma once
#include "EnemyMng.hpp"
#include "Vector2D.hpp"

struct tEnemyDataEx
{
	tEnemyData param;
	Vector2D pos, vspeed;
	double rad, hitRange;
	int time;
	bool isExist, isUngry, isHit, isMove;
};


// ==========================================================
class ZakoEnemy
{
public:
	virtual ~ZakoEnemy(){}
	virtual void Update() = 0;
	virtual void Draw() = 0;
	bool IsHit() {
		return data.isHit;
	}

protected:
	virtual void HitCheck() = 0;
	tEnemyDataEx data;
};


// ==========================================================
class ZakoEnemy_Ze : public ZakoEnemy
{
public:
	explicit ZakoEnemy_Ze(const tEnemyData& data);
	~ZakoEnemy_Ze();
	virtual void Update() override;
	virtual void Draw() override;

private:
	ZakoEnemy_Ze() : Speed(4), Brake(0.03), Easing(0.99999999) {}
	void Move0();
	virtual void HitCheck() override;

	const double Speed, Brake, Easing;
	
	int gh;
};


// ==========================================================
class ZakoEnemy_Career : public  ZakoEnemy
{
public:
	explicit ZakoEnemy_Career(const tEnemyData& data);
	~ZakoEnemy_Career();
	virtual void Update() override;
	virtual void Draw() override;

private:
	ZakoEnemy_Career(){}
	virtual void HitCheck() override;

	int gh[8];
};


// ==========================================================
class ZakoEnemy_Den : public ZakoEnemy
{
public:
	explicit ZakoEnemy_Den(const tEnemyData& data);
	~ZakoEnemy_Den();
	virtual void Update() override;
	virtual void Draw() override;

private:
	ZakoEnemy_Den(){}
	virtual void HitCheck() override;

	int gh[4];
};