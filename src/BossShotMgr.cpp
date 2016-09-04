#include "BossShotMgr.hpp"
#include "Eshot.hpp"
#include "NullEnemyShot.hpp"
#include "BossA.hpp"
#include "Stage.hpp"


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

	switch (IStage::GetNowStage())
	{
	case eStage::stage1:	mEnemyShot = (EnemyShot*) new Eshot;	break;
	case eStage::stage2:	break;
	case eStage::stage3:	break;
	case eStage::stage4:	break;
	case eStage::stage5:	break;
	case eStage::stage6:	break;
	case eStage::stage0:	break;
	default:				mEnemyShot = (EnemyShot*) new NullEnemyShot;
	}
}
// EOF