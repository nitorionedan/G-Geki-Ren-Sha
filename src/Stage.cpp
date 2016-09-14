#include "Stage.hpp"
#include "Keyboard.hpp"
#include "DebugMode.hpp"
#include "Game.hpp"

#include "Field.hpp"
#include "NullStage.hpp"
#include "OpeningStage.hpp"
#include "Stage1.hpp"

#include <DxLib.h>
#include <algorithm>
#include <cmath>
#include <cassert>

#undef min
#undef max

int Stage::s_time;
int Stage::s_rank;


Stage::Stage()
	: c_quake( new Counter(30) )
	, mField(static_cast<Field*>(new NullStage))
{
	hs_bgm;
	Screen = MakeScreen(640, 480, FALSE);
	
	// LoadStage(*.dat);		// TODO: こういう風にロードしたい 

	pos.SetVec(320., 240.);
	cycle = 0.;
	shake = 0.;
	f_quake = false;
	s_time = 0;
	s_rank = 0;
}


Stage::~Stage()
{
	Finalize();
}


void Stage::Finalize()
{
	DeleteSoundMem(hs_bgm);
}


void Stage::StageSet(eStage estage)
{
	if (hs_bgm != NULL)
		DeleteSoundMem(hs_bgm);

	delete mField;

	// ステージ用素材ロード
	switch (estage)
	{
	case eStage::opening:
		hs_bgm = LoadSoundMem("SOUND/s0.wav");
		mField = static_cast<Field*>(new OpenigStage);
		break;

	case eStage::stage1 :
		hs_bgm = LoadSoundMem("SOUND/s0.wav"); // TODO: change bgm
		mField = static_cast<Field*>(new Stage1);
		break;

	case eStage::stage2 :
		break;

	case eStage::stage3 :
		break;

	case eStage::stage4 :
		break;

	case eStage::stage5 :
		break;

	case eStage::stage6 :
		break;

	case eStage::stage0 :
		break;

	default: assert(!"Stage::StageSet()");
	}

	nowStage = estage;	// setting current stage

	Game::StageCall();
}


void Stage::Update()
{
	/* play the bgm */
	if (s_time == 0)
	{
		ChangeVolumeSoundMem(0, hs_bgm);
		PlaySoundMem(hs_bgm, DX_PLAYTYPE_LOOP);
	}

	// 時間経過
	s_time++;
	if (s_time % 120 == 0)	s_rank++;

	mField->Update();

	Quake();

	// TEST-----------------------------------------------------------------
	if (DebugMode::isTest == false)	return;

	/// TODO: 時がくるまで封印
//	if (effect->getIsAnime() && !f_quake)	f_quake = true;
	if (Keyboard::Instance()->isDown(KEY_INPUT_W)) cycle += 0.1;
	if (Keyboard::Instance()->isDown(KEY_INPUT_S)) cycle -= 0.1;
	if (Keyboard::Instance()->isDown(KEY_INPUT_D)) shake += 0.1;
	if (Keyboard::Instance()->isDown(KEY_INPUT_A)) shake -= 0.1;
}


void Stage::Draw()
{
	int tmpScreen = GetDrawScreen();
	SetDrawScreen(Screen);
	ClearDrawScreen();

	mField->Draw();

	SetDrawScreen(tmpScreen);
	DrawRasterScroll(pos.x, pos.y, cycle, shake, Screen, false);

	// TEST -------------------------------------------------------------------
	if (DebugMode::isTest == false)	return;

//	DrawFormatString(540, 20, GetColor(0, 255, 0), "TIME:%d sec", testTime);
	//DrawFormatString(520, 20, GetColor(0, 255, 0), "TIME:%d", s_time);
	DrawFormatString(520, 40, GetColor(0, 255, 0), "CYCLE:%lf", cycle); // 0.8 << good enough
	DrawFormatString(520, 60, GetColor(0, 255, 0), "SHAKE:%lf", shake); // 70 << good enough
}


void Stage::NextStage()
{
	StopSoundMem(hs_bgm); // stopping the bgm for next stage
	
	bool not_stage0 = (nowStage != eStage::stage0);
	if (not_stage0)
		nowStage = static_cast<eStage>(static_cast<int>(nowStage) + 1);
	else
		AllClear();
}


void Stage::Clear()
{
	// TODO: implement
	printfDx("Clear\n");
}


void Stage::AllClear()
{
	// TODO: implement
	printfDx("All Clear\n");
	nowStage = eStage::stage1;
}


void Stage::PlayQuake(){
	f_quake = true;
}


void Stage::Quake()
{
	if (f_quake == false)
		return; 
	
	c_quake->Update();
	
	double offsetB = GetRand(2) + 2;
	double offsetF = GetRand(2) + 2;
	
	if (c_quake->Remainder(4) >= 2)
		pos.x -= offsetB;

	if (c_quake->Remainder(4) <= 1)
		pos.x += offsetB;

	if (!c_quake->isLast()) return;		// 振動停止

	pos.SetVec(320., 240.);
	
	c_quake->Reset();
	f_quake = false;	// 振動をやめる
}


void Stage::SkipTo(int Time){
	s_time = Time;
}


// IStage============================================

std::shared_ptr<Stage> IStage::mStage;


void IStage::set(std::shared_ptr<Stage> stage)
{
	mStage = stage;
	assert(mStage != nullptr && "IStage::set()");
}


void IStage::Load(){
	mStage->StageSet( mStage->GetNowStage() );
}


void IStage::SkipTo(int Time){
	mStage->SkipTo(Time);
}
