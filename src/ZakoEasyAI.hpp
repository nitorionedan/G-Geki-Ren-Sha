#pragma once
#include "Enemy.hpp"


class EnemyZakoEasyAI
{
public:
	explicit EnemyZakoEasyAI(Enemy* enemy) {
		m_enemy = enemy;
	}
	virtual ~EnemyZakoEasyAI(){}
	virtual void Update() = 0;

protected:
	Enemy* m_enemy;
};


class ZakoAI_1 : public EnemyZakoEasyAI
{
public:
	ZakoAI_1();
	~ZakoAI_1();

private:

};