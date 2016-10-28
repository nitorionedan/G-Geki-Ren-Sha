#pragma once
#include "EnemyMng.hpp"
#include "Vector2D.hpp"

struct tEnemyDataEx
{
	tEnemyData param;
	Vector2D pos, vspeed;
	double rad, hitRange, vangle;
	int time, loopTime;
	bool isExist, isUngry, isHit, isMove;
};


// ==========================================================
class ZakoEnemy
{
public:
	virtual ~ZakoEnemy(){}
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual void UpdateTime() = 0;
	virtual void StartCheck() = 0;
	virtual void CalcDamage(int damage) = 0;
	virtual int GetElapsedTime() const = 0;
	virtual bool IsHit() const = 0;
	virtual bool HitCheck(const double& Range, const double X, const double Y, const double Damage) = 0;
	virtual bool HitCheck(const double X, const double Y, const double Damage) = 0;
	virtual bool IsWeak() const = 0;
	virtual bool IsExist() const = 0;

	tEnemyDataEx data;

protected:
	int maxHP;
	int id;

	static int num;
};


// ==========================================================
class ZakoEnemy_Ze : public ZakoEnemy
{
public:
	explicit ZakoEnemy_Ze(const tEnemyData& data);
	~ZakoEnemy_Ze();
	virtual void Update() override;
	virtual void Draw() override;
	virtual void UpdateTime() override {
		++data.time;
		data.loopTime = data.time % data.param.stop_time;
	}
	virtual void StartCheck() override;
	virtual int GetElapsedTime() const override {
		return data.time;
	}
	virtual bool HitCheck(const double& Range, const double X, const double Y, const double Damage) override;
	virtual bool HitCheck(const double X, const double Y, const double Damage) override;
	virtual bool IsHit() const override {
		return data.isHit;
	}
	virtual bool IsWeak() const override {
		return (data.param.hp <= maxHP / 3) ? true : false;
	}
	virtual bool IsExist() const override {
		return data.isExist;
	}

private:
	ZakoEnemy_Ze() : Speed(0), Brake(0), Easing(0) {}
	virtual void CalcDamage(int damage) override;

	void Move0();
	void Fire();

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
	virtual void UpdateTime() override {
		++data.time;
		data.loopTime = data.time % (data.param.stop_time + 160);
	}
	virtual void StartCheck() override;
	virtual int GetElapsedTime() const override {
		return data.time;
	}
	virtual bool HitCheck(const double& Range, const double X, const double Y, const double Damage) override;
	virtual bool HitCheck(const double X, const double Y, const double Damage) override;
	virtual bool IsHit() const {
		return data.isHit;
	}
	virtual bool IsWeak() const override {
		return (data.param.hp <= maxHP / 3) ? true : false;
	}
	virtual bool IsExist() const override {
		return data.isExist;
	}


private:
	ZakoEnemy_Career(){}
	virtual void CalcDamage(int damage) override;

	void Fire();
	void Move();

	int gh[8], gh_fire;
};


// ==========================================================
class ZakoEnemy_Den : public ZakoEnemy
{
public:
	explicit ZakoEnemy_Den(const tEnemyData& data);
	~ZakoEnemy_Den();
	virtual void Update() override;
	virtual void Draw() override;
	virtual void UpdateTime() override {
		++data.time;
		data.loopTime = data.time % data.param.stop_time;
	}
	virtual void StartCheck() override;
	virtual int GetElapsedTime() const override {
		return data.time;
	}
	virtual bool HitCheck(const double& Range, const double X, const double Y, const double Damage) override;
	virtual bool HitCheck(const double X, const double Y, const double Damage) override;
	virtual bool IsHit() const {
		return data.isHit;
	}
	virtual bool IsWeak() const override {
		return (data.param.hp <= maxHP / 3) ? true : false;
	}
	virtual bool IsExist() const override {
		return data.isExist;
	}

private:
	const double Move_max;

	ZakoEnemy_Den() : Move_max(0) {}
	virtual void CalcDamage(int damage) override;

	void Move0();
	void Move1();
	void Fire0();

	double distance, brake;
	int gh[4];
	int sh_shot;
	int returnTime;
	bool isReturn;
	bool isFire;
};


// ==========================================================
class ZakoEnemy_Flower : public ZakoEnemy
{
public:
	explicit ZakoEnemy_Flower(const tEnemyData& data);
	~ZakoEnemy_Flower();
	virtual void Update() override;
	virtual void Draw() override;
	virtual void UpdateTime() override {
		++data.time;
		data.loopTime = data.time % data.param.stop_time;
	}
	virtual void StartCheck() override;
	virtual int GetElapsedTime() const override {
		return data.time;
	}
	virtual bool HitCheck(const double& Range, const double X, const double Y, const double Damage) override;
	virtual bool HitCheck(const double X, const double Y, const double Damage) override;
	virtual bool IsHit() const {
		return data.isHit;
	}
	virtual bool IsWeak() const override {
		return (data.param.hp <= maxHP / 3) ? true : false;
	}
	virtual bool IsExist() const override {
		return data.isExist;
	}

private:
	const double Max_speed_x;
	const double Max_speed_y;

	ZakoEnemy_Flower() : Max_speed_x(0), Max_speed_y(0) {}
	virtual void CalcDamage(int damage) override;

	void Move0();
	void Move1();
	void Move2();
	void Move3();
	void Fire0();

	double radAng, radAng_petal;
	double brake, addAng;
	int gh_core,
		gh_petal,
		gh_missile,
		gh_all; // ‚¢‚é‚©H
	bool hasMissile;
	bool isReturn;
};