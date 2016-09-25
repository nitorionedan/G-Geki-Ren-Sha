#include "Stage.hpp"
#include "Keyboard.hpp"
#include "DebugMode.hpp"
#include "BossChara.hpp"
#include "EnemyMng.hpp"
#include "Sound.hpp"

/* field */
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
	Screen = MakeScreen(640, 480, FALSE);

	// LoadStage(*.dat);		// TODO: こういう風にロードしたい 

	tCamera.pos = GetCameraPosition();
	tCamera.tang = GetCameraAngleVRotate();
	tCamera.hang = GetCameraAngleHRotate();
	tCamera.tang = GetCameraAngleTRotate();

	Initialize();
}


Stage::~Stage()
{
	Finalize();
}


void Stage::Initialize()
{
	state = eState::game;

	time = 0;
	rank = 0;
	c_bossBgm = 0;
	pos.SetVec(320., 240.);
	f_quake = false;
	isStanby = true;
}


void Stage::Finalize()
{
}


void Stage::StageSet(eStage estage)
{
	delete mField;
	mField = static_cast<Field*>(new NullStage);

	// ステージ用素材ロード
	switch (estage)
	{
	case eStage::opening:
		mField = static_cast<Field*>(new OpenigStage);
		Sound::Play(eSound::opening);
		break;
	case eStage::stage1 :
		mField = static_cast<Field*>(new Stage1);
		Sound::Play(eSound::stage1);
		break;
	case eStage::stage2 :
		Sound::Play(eSound::stage2);
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
	isStanby = true;
}


void Stage::Update()
{
	switch (state)
	{
	case Stage::eState::game:
		UpdateField();
		if (IEnemyMng::IsBossZone())
		{
			if (Sound::IsPlaySome() == false)
				++c_bossBgm;

			if (c_bossBgm == 120)
			{
				switch (nowStage)
				{
				case eStage::opening:
					Sound::Play(eSound::bossA);
					break;
				case eStage::stage1:
					Sound::Play(eSound::bossB);
					break;
				case eStage::stage2:
					Sound::Play(eSound::bossC);
					break;
				case eStage::stage3:
					Sound::Play(eSound::bossD);
					break;
				case eStage::stage4:
					Sound::Play(eSound::bossE);
					break;
				case eStage::stage5:
					Sound::Play(eSound::bossF);
					break;
				case eStage::stage6:
					Sound::Play(eSound::bossG);
					break;
				case eStage::stage0:
					Sound::Play(eSound::bossH);
					break;
				}
				c_bossBgm = 0;
			}
		}
		break;
	case Stage::eState::result:
		Update_Result();
		break;
	}

	// TEST-----------------------------------------------------------------
	if (DebugMode::isTest == false)	return;

	/// TODO: 時がくるまで封印
//	if (effect->getIsAnime() && !f_quake)	f_quake = true;
	if (Keyboard::Instance()->isPush(KEY_INPUT_W))
		Sound::FadeOutStop();
}


void Stage::Draw()
{
	switch (state)
	{
	case Stage::eState::game:
		mField->Draw();
		DrawStageCall();
		break;
	case Stage::eState::result:
		DrawResult();
		break;
	}

	// TEST -------------------------------------------------------------------
	if (DebugMode::isTest == false)	return;

	//DrawFormatString(540, 20, GetColor(0, 255, 0), "TIME:%d sec", testTime);
	//DrawFormatString(520, 20, GetColor(0, 255, 0), "TIME:%d", time);
	//DrawFormatString(520, 40, GetColor(0, 255, 0), "CYCLE:%lf", cycle); // 0.8 << good enough
}

void Stage::UpdateField()
{
	if (time == StageCallTime) // stop to StageCall
		isStanby = false;

	// ステージの経過時間
	++time;

	// rank increment
	const int& RankUpTime = (time % 120 == 0);
	if (RankUpTime)
		++rank;

	mField->Update();

	Quake();
}


void Stage::NextStage()
{	
	bool not_stage0 = (nowStage != eStage::stage0);
	if (not_stage0)
		nowStage = static_cast<eStage>(static_cast<int>(nowStage) + 1);
	else
		AllClear();
	Initialize();
}


void Stage::Clear()
{
	// TODO: implement
	printfDx("Clear\n");
	
	Sound::FadeOutStop();
	
	state = eState::result;
	time = 0;
}


void Stage::AllClear()
{
	// TODO: implement
	printfDx("All Clear\n");
	nowStage = eStage::opening;
}


void Stage::PlayQuake(){
	f_quake = true;
}


void Stage::Update_Result()
{
	++time;

	if (time == 300)
		NextStage();
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


void Stage::DrawResult()
{
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
