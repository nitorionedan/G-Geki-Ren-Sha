#include "BossShotMgr.hpp"
#include "Eshot.hpp"
#include "NullEnemyShot.hpp"
#include "BossA.hpp"


BossShotMgr::BossShotMgr(EnemyShot* mEnemyShot)
	: mEnemyShot(mEnemyShot)
{
}

BossShotMgr::~BossShotMgr(){
	delete mEnemyShot;
}

void BossShotMgr::Update(const Player& player){
	mEnemyShot->Update(player);
}

void BossShotMgr::Draw(){
	mEnemyShot->Draw();
}


void BossShotMgr::ChangeShot(eEnemyShot type)
{
	delete mEnemyShot;
	switch (type)
	{
	case eEnemyShot::ShotA :
		mEnemyShot = (EnemyShot*) new Eshot;
		break;
	case eEnemyShot::None :
		mEnemyShot = (EnemyShot*) new NullEnemyShot;
		break;
	}
}
// EOF