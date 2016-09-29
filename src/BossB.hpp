#pragma once
#include "BossTask.hpp"
#include <memory>


class BossB : public BossTask
{
	enum class ePart
	{
		head,
		leftWing,
		rightWing,
		tail,
	};

	class Body
	{
	public:
		explicit Body(ePart part);
		~Body();
		void Update();
		void Draw();
		
		int gh;
		int part;

	private:
		Body(){}
		void Initialize();
		
		float hp;
	};

public:
	BossB();
	~BossB();
	virtual void Update() override;
	virtual void Draw() override;
	virtual const Vector2D& GetPos() const override;
	virtual bool HitCheck(const double& ColX, const double& ColY, const int& DamagePoint) override;
	virtual const bool IsDead() const override;

private:
	std::unique_ptr<Body> body_head;
	std::unique_ptr<Body> body_leftWing;
	std::unique_ptr<Body> body_rightWing;
	std::unique_ptr<Body> body_tail;
};