#include "Stage.hpp"
#include "Keyboard.hpp"
#include "DebugMode.hpp"
#include "Game.hpp"

#include "Field.hpp"
#include "NullStage.hpp"
#include "OpeningStage.hpp"
#include "Stage1.hpp"

#include <algorithm>
#include <cmath>
#include <cassert>

#undef min
#undef max

constexpr float CamX = 320.f;
constexpr float CamY = 240.f;
constexpr float CamZ = -415.6922f;
constexpr int StageCallTime = 120;

Stage::Stage()
	: c_quake( new Counter(30) )
	, graphic(new Graphic)
	, mField(static_cast<Field*>(new NullStage))
{
	hs_bgm;
	Screen = MakeScreen(640, 480, FALSE);
	
	// LoadStage(*.dat);		// TODO: こういう風にロードしたい 

	tCamera.pos = GetCameraPosition();
	tCamera.tang = GetCameraAngleVRotate();
	tCamera.hang = GetCameraAngleHRotate();
	tCamera.tang = GetCameraAngleTRotate();

	pos.SetVec(320., 240.);
	cycle = 0.;
	shake = 0.;
	f_quake = false;
	fadeoutFlag = false;
	fadeinFlag = false;
	isStanby = true;

	time = 0;
	rank = 0;
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
		hs_bgm = LoadSoundMem("SOUND/s1.wav");
		ChangeVolumeSoundMem(0, hs_bgm); // 170
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

	PlaySoundMem(hs_bgm, DX_PLAYTYPE_LOOP);
	nowStage = estage;	// setting current stage
	isStanby = true;
}


void Stage::Update()
{
	if (time == StageCallTime) // stop to StageCall
		isStanby = false;

	// 時間経過
	++time;

	// rank increment
	const int& RankUpTime = (time % 120 == 0);
	if (RankUpTime)
		++rank;

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
	mField->Draw();
	DrawStageCall();

	// TEST -------------------------------------------------------------------
	if (DebugMode::isTest == false)	return;

//	DrawFormatString(540, 20, GetColor(0, 255, 0), "TIME:%d sec", testTime);
	//DrawFormatString(520, 20, GetColor(0, 255, 0), "TIME:%d", time);
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
	time = 0;
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


void Stage::DrawStageCall()
{
	/* Stage call */
	if (isStanby == false)
		return;

	const int&    X_MSG = 290, Y_MSG = 240, SPACE_MSG = 16;
	const double& EXRATE_MSG = 2.0;

	switch (nowStage)
	{
	case eStage::opening:
		graphic->DrawMyString2(X_MSG - 20, Y_MSG, "OPENING!", SPACE_MSG, true, EXRATE_MSG);
		break;
	case eStage::stage1:
		graphic->DrawMyString2(X_MSG, Y_MSG, "STAGE 1", SPACE_MSG, true, EXRATE_MSG);
		break;
	case eStage::stage2:
		graphic->DrawMyString2(X_MSG, Y_MSG, "STAGE 2", SPACE_MSG, true, EXRATE_MSG);
		break;
	case eStage::stage3:
		graphic->DrawMyString2(X_MSG, Y_MSG, "STAGE 3", SPACE_MSG, true, EXRATE_MSG);
		break;
	case eStage::stage4:
		graphic->DrawMyString2(X_MSG, Y_MSG, "STAGE 4", SPACE_MSG, true, EXRATE_MSG);
		break;
	case eStage::stage5:
		graphic->DrawMyString2(X_MSG, Y_MSG, "STAGE 5", SPACE_MSG, true, EXRATE_MSG);
		break;
	case eStage::stage6:
		graphic->DrawMyString2(X_MSG, Y_MSG, "STAGE 6", SPACE_MSG, true, EXRATE_MSG);
		break;
	case eStage::stage0:
		graphic->DrawMyString2(X_MSG, Y_MSG, "STAGE 0", SPACE_MSG, true, EXRATE_MSG);
		break;
	}

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

	if (!c_quake->isLast()) return;		// stop to quake

	pos.SetVec(320., 240.);
	
	c_quake->Reset();
	f_quake = false;	// 振動をやめる
}


void Stage::SkipTo(int Time){
	time = Time;
}


void Stage::Fadein()
{
	fadeinFlag = true;
	SetVolumeSoundMem(0, hs_bgm);
}


void Stage::Fadeout()
{
	fadeinFlag = true;
	SetVolumeSoundMem(0, hs_bgm);
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
