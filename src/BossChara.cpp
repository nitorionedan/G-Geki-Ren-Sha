//------------------------------------------------------------------------------
/// @file
/// @brief		BossChara.hppの実装
/// @attention  このファイルの利用は、同梱のREADMEにある
///             利用条件に従ってください

//------------------------------------------------------------------------------

#include "BossChara.hpp"
#include "NullBoss.hpp"
#include "BossA.hpp"
#include "BossB.hpp"
#include "Stage.hpp"
#include "BossShotMgr.hpp"
#include <cassert>


BossChara::BossChara(BossTask* mBoss_)
	: mBoss(mBoss_)
{
}


BossChara::~BossChara(){
	delete mBoss;
}


bool BossChara::HitCheck(const double& ColX, const double& ColY, const int& DamagePoint){
	return mBoss->HitCheck(ColX, ColY, DamagePoint) ? true : false;
}


void BossChara::Start(eBoss_num num)
{
	delete mBoss;
	switch (num)
	{
	case eBoss_A:
		mBoss = static_cast<BossTask*>(new BossA);
		IBossShotMgr::ChangeShot(eEnemyShot::ShotA);
		break;
	case eBoss_B:
		mBoss = static_cast<BossTask*>(new BossB);
		IBossShotMgr::ChangeShot(eEnemyShot::None);
		break;
	case eBoss_C:
		break;
	case eBoss_D:
		break;
	case eBoss_E:
		break;
	case eBoss_F:	
		break;
	case eBoss_G:	
		break;
	case eBoss_None:
		mBoss = new NullBoss;
		IBossShotMgr::ChangeShot(eEnemyShot::None);
		break;
	default:	assert(!"BossChara::Start()");
	}
}


//===================================================================
std::shared_ptr<BossChara> IBossChara::mBossChara;


void IBossChara::set(std::shared_ptr<BossChara> bossChara)
{
	mBossChara = bossChara;
	assert(mBossChara != nullptr && "IBossChara::set()");
}


void IBossChara::Start()
{
	switch (IStage::GetNowStage())
	{
	case eStage::opening: mBossChara->Start(eBoss_B); break;
	case eStage::stage1: mBossChara->Start(eBoss_B); break;
	case eStage::stage2:	break;
	case eStage::stage3:	break;
	case eStage::stage4:	break;
	case eStage::stage5:	break;
	case eStage::stage6:	break;
	case eStage::stage0:	break;
	default:	assert(!"IBossChara::Start()");
	}
}


bool IBossChara::HitCheck(const double & ColX, const double & ColY, const int & DamagePoint){
	return mBossChara->HitCheck(ColX, ColY, DamagePoint);
}
