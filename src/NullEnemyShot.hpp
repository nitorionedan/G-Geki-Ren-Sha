#pragma once

#include "EnemyShot.hpp"


/// nullptr�̑���
class NullEnemyShot : public EnemyShot
{
public:
	NullEnemyShot(){}
	~NullEnemyShot(){}
	virtual void Update(const Player& player) override {}	// �������Ȃ�
	virtual void Draw() override {}		// �������Ȃ�
};