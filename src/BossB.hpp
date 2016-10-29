#pragma once
#include "BossTask.hpp"
#include "Image.hpp"
#include <memory>


class BossB : public BossTask
{
	enum class eState
	{
		start,
		normal,
		stay,
		weak,
		dead,
	};

	enum class ePart
	{
		head,
		leftWing,
		rightWing,
		tail00,
		tail01,
		gun,
	};

	class Body
	{
	public:
		explicit Body(ePart part);
		~Body();
		void Update();
		void Draw(const BossB& boss);
		void Damage(int point);
		bool HitCheck(const double& ColX, const double& ColY, const int& DamagePoint);
		void Fire();
		void Fire_Head();
		void Fire_Gun();
		void Fire_Tail00();
		void Fire_Tail01();
		
		ePart part;
		Vector2D pos;
		double hitRange;
		double hp, max_hp;
		bool isDamage;
		bool isExist;
		int c_dead;
		int elapsedTime;

	private:
		Body(){}
		void Initialize();
		int sh_missile;
	};

public:
	BossB();
	~BossB();
	virtual void Update() override;
	virtual void Draw() override;
	virtual const Vector2D& GetPos() const override;
	virtual bool HitCheck(const double& ColX, const double& ColY, const int& DamagePoint) override;
	virtual const bool IsDead() const override;
	void Update_Start();
	void Update_Normal();
	void Update_Stay();
	void Update_Weak();
	void Update_Dead();
	void Fire_Normal();
	void Fire_Stay();
	void Fire_Weak();

private:
	std::unique_ptr<Body> body_head;
	std::unique_ptr<Body> body_leftWing;
	std::unique_ptr<Body> body_rightWing;
	std::unique_ptr<Body> body_tail00;
	std::unique_ptr<Body> body_tail01;
	std::unique_ptr<Body> body_gun00;
	std::unique_ptr<Body> body_gun01;
	std::unique_ptr<Body> body_gun02;
	std::unique_ptr<Body> body_gun03;
	std::unique_ptr<Image> img;
	Vector2D pos_backWing;
	eState state;

	double c_move, c_move_y;
	int elapsedTime;
	int c_start, c_normal, c_stay, c_weak, c_dead;
	int c_fire_normal, c_fire_stay, c_fire_weak;
	int alpha_gun;
	bool readyWing;
	bool isExist, isDead;
	int hp;
	int sh_dead;
};