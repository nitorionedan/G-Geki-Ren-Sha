#pragma once

#include "EnemyShot.hpp"


/// nullptrの代わり
class NullEnemyShot : public EnemyShot
{
public:
	NullEnemyShot(){}
	~NullEnemyShot(){}
	virtual void Update(const Player& player) override {}	// 何もしない
	virtual void Draw() override {}		// 何もしない
};