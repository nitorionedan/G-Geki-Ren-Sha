#include "Stage.hpp"
#include "Keyboard.hpp"
#include "DebugMode.hpp"
#include "BossChara.hpp"
#include "EnemyMng.hpp"
#include "Sound.hpp"
#include "IScore.hpp"

/* field */
#include "Field.hpp"
#include "NullStage.hpp"
#include "OpeningStage.hpp"
#include "Stage1.hpp"
#include "Stage2.hpp"

#include <algorithm>
#include <cmath>
#include <cassert>

#undef min
#undef max

namespace
{
	constexpr float CamX = 320.f;
	constexpr float CamY = 240.f;
	constexpr float CamZ = -415.6922f;
	constexpr int StageCallTime = 120;
	constexpr int RESULT_TIME = 1300;
}


bool Stage::s_isContinue = false;


Stage::Stage()
	: c_quake( new Counter(30) )
	, graphic( new Graphic )
	, mField( static_cast<Field*>(new NullStage) )
	, c_fade(0)
{
	Screen = MakeScreen(640, 480, TRUE);
	gh_black = LoadGraph("GRAPH/GAME/BACKGROUND/blackbg.png");

	// LoadStage(*.dat);		// TODO: こういう風にロードしたい 

	tCamera.pos = GetCameraPosition();
	tCamera.tang = GetCameraAngleVRotate();
	tCamera.hang = GetCameraAngleHRotate();
	tCamera.tang = GetCameraAngleTRotate();

	Initialize();
}


Stage::~Stage()
{
	DeleteGraph(gh_black);
	Finalize();
}


void Stage::Initialize()
{
	state = eState::game;
	quakeType = eQuake::normal;

	if ( s_isContinue )
	{
		int h_file = FileRead_open("./data/continue.dat");
		if (h_file != 0) // successful
		{
			while (1)
			{
				char tmpC = FileRead_getc(h_file);
				if (tmpC == EOF)
					break;
				nowStage = static_cast<eStage>( std::atoi(&tmpC) );
			}
		}
		else // failed
			nowStage = eStage::opening;
		FileRead_close(h_file);
		s_isContinue = false;
	}

	time = 0;
	rank = 0;
	c_bossBgm = 0;
	start_score = 0;
	end_score = 0;
	pos = Vector2D::ZERO;
	f_quake = false;
	isStanby = true;
	StageSet(nowStage);
}


void Stage::Finalize()
{
}


void Stage::StageSet(eStage estage)
{
	delete mField;
	
	// ステージ用素材ロード
	switch (estage)
	{
	case eStage::opening:
		mField = static_cast<Field*>(new OpenigStage);
		Sound::Play(eSound::opening);
		break;
	case eStage::stage1 :
		mField = static_cast<Field*>(new Stage2);
		Sound::Play(eSound::stage2);
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
	if (time == 0)
		start_score = IScore::GetScore();

	switch (state)
	{
	case Stage::eState::game:
		UpdateField();

		if (c_fade < 255)
			++c_fade;

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
		UpdateField();
		Update_Result();
		break;
	}

	// TEST-----------------------------------------------------------------
	if (DebugMode::isTest == false)	return;

	/// TODO: 時がくるまで封印
//	if (effect->getIsAnime() && !f_quake)	f_quake = true;
}


void Stage::Draw()
{
	int tmpSc = NULL;
	switch (state)
	{
	case Stage::eState::game:
		tmpSc = GetDrawScreen();
		SetDrawScreen(Screen);
		ClearDrawScreen();

		mField->Draw();

		SetDrawScreen(DX_SCREEN_BACK);
		DrawGraph(pos.x, pos.y, Screen, TRUE);
		SetDrawScreen(tmpSc);

		DrawStageCall();
		break;
	case Stage::eState::result:
		mField->Draw();

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - c_fade);
		DrawGraph(0, 0, gh_black, FALSE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		DrawResult();
		break;
	}

	// TEST -------------------------------------------------------------------
	if (DebugMode::isTest == false)	return;

	//DrawFormatString(540, 20, GetColor(0, 255, 0), "TIME:%d sec", testTime);
	//DrawFormatString(520, 20, GetColor(0, 255, 0), "TIME:%d", time);
	//DrawFormatString(520, 40, GetColor(0, 255, 0), "pos.x:%lf", pos.x); // 0.8 << good enough
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

	switch (quakeType)
	{
	case eQuake::smal:	 SmallQuale(); break;
	case eQuake::normal: Quake();      break;
	case eQuake::big:	 BigQuake();   break;
	}
}


void Stage::NextStage()
{	
	bool not_stage0 = (nowStage != eStage::stage0);
	if (not_stage0)
	{
		nowStage = static_cast<eStage>(static_cast<int>(nowStage) + 1);
		IEnemyMng::Load(nowStage);
	}
	else
		AllClear();
	Initialize();
}


void Stage::Clear()
{	
	Sound::FadeOutStop();
	
	state = eState::result;
	time = 0;
	end_score = IScore::GetScore();
	sum_score = end_score - start_score;
}


void Stage::AllClear()
{
	// TODO: implement
	printfDx("All Clear\n");
	nowStage = eStage::opening;
}


void Stage::PlayQuake() {
	f_quake = true;
}

void Stage::PlayQuake(eQuake aukeType)
{
	this->quakeType = quakeType;
	f_quake = true;
}


void Stage::Update_Result()
{
	++time;

	/* Blackout */
	if (c_fade > 0)
		--c_fade;
	if (c_fade < 0)
		c_fade = 0;

	if (time == RESULT_TIME)
		NextStage();
}


void Stage::DrawStageCall()
{
	/* Stage call */
	if (isStanby == false)
		return;

	const int&    X_MSG = 290, Y_MSG = 240, SPACE_MSG = 16;
	const double& EXRATE_MSG = 2.;

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
	std::string stageName;

	switch (nowStage)
	{
	case eStage::opening:
		stageName = "OPENING SCORE";
		break;
	case eStage::stage1:
		stageName = "STAGE 1 SCORE";
		break;
	case eStage::stage2:
		stageName = "STAGE 2 SCORE";
		break;
	case eStage::stage3:
		stageName = "STAGE 3 SCORE";
		break;
	case eStage::stage4:
		stageName = "STAGE 4 SCORE";
		break;
	case eStage::stage5:
		stageName = "STAGE 5 SCORE";
		break;
	case eStage::stage6:
		stageName = "STAGE 6 SCORE";
		break;
	case eStage::stage0:
		stageName = "STAGE 0 SCORE";
		break;
	}

	graphic->DrawMyString2(100, 240, stageName.c_str(), 16, false, 2.);

	if (time >= 300)
		graphic->DrawScore(480, 240, sum_score, 16, 2.);
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

	if (!c_quake->isLast())
		return;		// stop to quake

	pos = Vector2D::ZERO;
	
	c_quake->Reset();
	f_quake = false;	// 振動をやめる
}


void Stage::SmallQuale()
{
	if (f_quake == false)
		return;

	c_quake->Update();

	double offsetB = GetRand(1) + 1;
	double offsetF = GetRand(1) + 1;

	if (c_quake->Remainder(4) >= 2)
		pos.x -= offsetB;

	if (c_quake->Remainder(4) <= 1)
		pos.x += offsetB;

	if (!c_quake->isLast())
		return;		// stop to quake

	pos = Vector2D::ZERO;

	c_quake->Reset();
	f_quake = false;
}


void Stage::BigQuake()
{
	if (f_quake == false)
		return;

	c_quake->Update();

	double offsetB = GetRand(4) + 4; // back
	double offsetF = GetRand(4) + 4; // fore
	
	if (c_quake->Remainder(4) >= 2)
	{
		pos.x += GetRand(4);
		pos.y -= GetRand(4);
	}

	if (c_quake->Remainder(4) <= 1)
	{
		pos.x -= GetRand(4);
		pos.y += GetRand(4);
	}

	if (!c_quake->isLast())
		return;		// stop to quake

	pos = Vector2D::ZERO;

	c_quake->Reset();
	f_quake = false;
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


void IStage::Quake(eQuake quakeType)
{
	switch (quakeType)
	{
	case eQuake::smal:
		mStage->PlayQuake(eQuake::smal);
		break;
	case eQuake::normal:
		mStage->PlayQuake(eQuake::normal);
		break;
	case eQuake::big:
		mStage->PlayQuake(eQuake::big);
		break;
	default: assert(!"");
	}
}


void IStage::SkipTo(int Time){
	mStage->SkipTo(Time);
}
