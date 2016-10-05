#pragma once
#include "EnemyAI.hpp"


class BossAAI_start : public EnemyAI
{
public:
	BossAAI_start();
	virtual void Update() override;

private:
	int c_start;
};


class BossAAI_normal : public EnemyAI
{
public:
	virtual void Update() override;
};


class BossAAI_weak : public EnemyAI
{
public:
	virtual void Update() override;
};


class BossAAI_dead : public EnemyAI
{
public:
	virtual void Update() override;
};


class BossAAI_end : public EnemyAI
{
public:
	virtual void Update() override;
};