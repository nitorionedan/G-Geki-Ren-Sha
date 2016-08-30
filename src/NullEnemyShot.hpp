#pragma once

#include "EnemyShot.hpp"


/// nullptr‚Ì‘ã‚í‚è
class NullEnemyShot : public EnemyShot
{
public:
	NullEnemyShot(){}
	~NullEnemyShot(){}
	virtual void Update(const Player& player) override {}	// ‰½‚à‚µ‚È‚¢
	virtual void Draw() override {}		// ‰½‚à‚µ‚È‚¢
};