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
	virtual void ResetState() = 0;
	virtual void StartCheck() = 0;
	virtual void UngryCheck() = 0;
	virtual int GetElapsedTime() const = 0;
	virtual bool IsHit() const = 0;
	virtual bool HitCheck(const double& Range, const Vector2D pos, const double& damage) = 0;
	virtual bool HitCheck(const double& Range, const double X, const double Y, const double Damage) = 0;
	virtual bool HitCheck(const double X, const double Y, const double Damage) = 0;
	virtual bool IsWeak() const = 0;
	virtual bool IsExist() const = 0;

protected:
	virtual void CalcDamage(int damage) = 0;
	tEnemyDataEx data;
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
	virtual void ResetState() override {
		data.isHit = false;
	}
	virtual int GetElapsedTime() const override {
		return data.time;
	}
	virtual void UngryCheck() override;
	virtual bool HitCheck(const double& Range, const Vector2D pos, const double& damage) override;
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
	ZakoEnemy_Ze() : Speed(4), Brake(0.03), Easing(0.99999999) {}
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
	virtual void ResetState() override {
		data.isHit = false;
	}
	virtual int GetElapsedTime() const override {
		return data.time;
	}
	virtual void UngryCheck() override;
	virtual bool HitCheck(const double& Range, const Vector2D pos, const double& damage) override;
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
	virtual void ResetState() override {
		data.isHit = false;
	}
	virtual int GetElapsedTime() const override {
		return data.time;
	}
	virtual void UngryCheck() override;
	virtual bool HitCheck(const double& Range, const Vector2D pos, const double& damage) override;
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
	ZakoEnemy_Den(){}
	virtual void CalcDamage(int damage) override;

	void Move();
	void Fire();

	int gh[4];
};