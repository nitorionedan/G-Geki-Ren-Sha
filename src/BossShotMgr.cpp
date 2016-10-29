#include "BossShotMgr.hpp"
#include "Eshot.hpp"
#include "NullEnemyShot.hpp"
#include "BossA.hpp"
#include "Stage.hpp"
#include <cassert>


BossShotMgr::BossShotMgr(EnemyShot* mEnemyShot)
	: mEnemyShot(mEnemyShot)
{
}


BossShotMgr::~BossShotMgr(){
	delete mEnemyShot;
}


void BossShotMgr::Update(const Player& player)
{
	if (mEnemyShot == nullptr)
		return;

	mEnemyShot->Update(player);
}


void BossShotMgr::Draw() 
{
	if (mEnemyShot == nullptr)
		return;

	mEnemyShot->Draw();
}


void BossShotMgr::ChangeShot(eEnemyShot type)
{
	delete mEnemyShot;
	mEnemyShot = nullptr;

	switch (type)
	{
	case eEnemyShot::ShotA:
		mEnemyShot = (EnemyShot*)new Eshot;
		break;
	case eEnemyShot::ShotB:
		break;
	case eEnemyShot::ShotC:
		break;
	case eEnemyShot::ShotD:
		break;
	case eEnemyShot::ShotE:
		break;
	case eEnemyShot::ShotF:
		break;
	case eEnemyShot::ShotZ:
		break;
	case eEnemyShot::None:
		break;
	default: assert(!"BossShotMgr::ChangeShot()");
	}
}


// ===============================================================
std::shared_ptr<BossShotMgr> IBossShotMgr::mBossShot;


void IBossShotMgr::set(std::shared_ptr<BossShotMgr> bossShot)
{
	mBossShot = bossShot;
}


void IBossShotMgr::ChangeShot(eEnemyShot type)
{
	mBossShot->ChangeShot(type);
}