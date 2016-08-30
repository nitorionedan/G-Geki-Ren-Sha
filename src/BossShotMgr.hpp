#pragma once

#include "BossChara.hpp"
#include "EnemyShot.hpp"
#include "Player.hpp"


enum class eEnemyShot
{
	ShotA,
	ShotB,
	ShotC,
	ShotD,
	ShotE,
	ShotF,
	ShotZ,

	None,
};


class BossShotMgr
{
public:
	BossShotMgr(EnemyShot* mEnemyShot);
	~BossShotMgr();

	void Update(const Player& player);
	void Draw();
	void ChangeShot(eEnemyShot chara);

private:
	EnemyShot* mEnemyShot;
};