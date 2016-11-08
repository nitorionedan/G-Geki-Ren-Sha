#include "Stage.hpp"
#include "Keyboard.hpp"
#include "DebugMode.hpp"
#include "BossChara.hpp"
#include "EnemyMng.hpp"
#include "Sound.hpp"
#include "IScore.hpp"
#include "Graphics2D.hpp"
#include "FileDef.h"

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
	: c_quake(new Counter(30))
	, graphic(new Graphic)
	, mField(static_cast<Field*>(new NullStage))
	, dist(new Distortion)
	, c_fade(0)
{
	Screen   = MakeScreen(640, 480, TRUE);
	gh_black = LoadGraph(MyFile::Gr::PLANE_BLACK);
	gh_tips00 = LoadGraph(MyFile::Gr::TIPS00);
	gh_tips01 = LoadGraph(MyFile::Gr::TIPS01);

	tCamera.pos  = GetCameraPosition();
	tCamera.tang = GetCameraAngleVRotate();
	tCamera.hang = GetCameraAngleHRotate();
	tCamera.tang = GetCameraAngleTRotate();

	Initialize();
}


Stage::~Stage()
{
	delete mField;
	DeleteGraph(gh_tips00);
	DeleteGraph(gh_tips01);
	DeleteGraph(gh_black);
	Finalize();
}


void Stage::Initialize()
{
	state = eState::game;
	quakeType = eQuake::normal;

	if ( s_isContinue )
	{
		int h_file = FileRead_open(MyFile::Dat::CONTINUE);
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
			nowStage = eStage::stage1;
		FileRead_close(h_file);
		s_isContinue = false;
	}

	time        = 0;
	rank        = 0;
	c_bossBgm   = 0;
	c_result    = 0;
	start_score = 0;
	end_score   = 0;
	pos         = Vector2D::ZERO;
	f_quake     = false;
	isStanby    = true;
	f_AllClear  = false;
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

	dist->Update();

	// TEST-----------------------------------------------------------------
	if (DebugMode::isTest == false)	return;
	DrawFormatString(400, 30, GetColor(0, 255, 0), "TIME:%d", time);

	/// TODO: 時がくるまで封印
//	if (effect->getIsAnime() && !f_quake)	f_quake = true;
}


void Stage::Draw()
{
	SetDrawScreen(Screen);
	ClearDrawScreen();

	switch (state)
	{
	case Stage::eState::game:
		mField->Draw();

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - c_fade);
		DrawGraph(0, 0, gh_black, FALSE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		if (nowStage == eStage::opening)
		{
			/* Tips */
			if (time >= 1 && time <= 240)
				DrawRotaGraph(320, 240, 2, 0, gh_tips00, TRUE);
			if (time >= 241 && time <= 480)
				DrawRotaGraph(320, 240, 2, 0, gh_tips01, TRUE);
		}

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

	SetDrawScreen(DX_SCREEN_BACK);
	dist->Draw(Screen);

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
	bool not_stg0 = (nowStage != eStage::stage0);
	bool not_stg2 = (nowStage != eStage::stage1);

	if (not_stg2)
	{
		nowStage = static_cast<eStage>(static_cast<int>(nowStage) + 1);
		IEnemyMng::Load(nowStage);
		Initialize();
	}
	else
		AllClear();
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
	///nowStage = eStage::opening;
	f_AllClear = true;
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
	++c_result;

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
		graphic->DrawMyString2(100, 210, "s CONGRATULATIONS! s", 16, false, 2.);
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

	/* The Life Bonus */
	int lifeBonus = IPlayer::GetLife() * 64864; // 虫は無視（激寒）
	graphic->DrawMyString2(100, 180, "LIFE BONUS", 16, true, 2.);
	graphic->DrawScore(480, 180, lifeBonus, 16, 2.);
	if (c_result == 1)
		IScore::AddScore(lifeBonus);

	graphic->DrawMyString2(100, 240, stageName.c_str(), 16, true, 2.);

	/* The Pacifist Bonus */
	if (IEnemyMng::CheckPacifist())
	{
		int pci_score = 10140 * (static_cast<int>(nowStage) + 1);
		graphic->DrawMyString2(100, 280, "PACIFIST BONUS", 16, true, 2.);
		graphic->DrawScore(480, 280, pci_score, 16, 2.);
		if (c_result == 1)
			IScore::AddScore(pci_score);
	}

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


void Stage::PlayDistortion(double x, double y) {
	dist->PlayDistortion(x, y);
}


void Stage::SkipTo(int Time) {
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


// ==========================================-
Stage::Distortion::Distortion()
{
	screen = MakeScreen(640, 480, TRUE);
}


Stage::Distortion::~Distortion()
{
}


void Stage::Distortion::Update()
{
	for (auto& i : m_dist)
		++i.time;

	for (auto& i : m_dist)
		i.range += 6;

	// erase
	m_dist.erase(std::remove_if(
		std::begin(m_dist), std::end(m_dist),
		[](tDist& dist) {
			return (dist.time >= 30);
		}),
		std::end(m_dist));
}


void Stage::Distortion::Draw(const int & GrHandle)
{
	for(auto i : m_dist)
	{
		SetDrawScreen(i.sc);
		ClearDrawScreen();

		DrawGraph(0, 0, GrHandle, TRUE);

		SetDrawScreen(DX_SCREEN_BACK);
		// 画面を歪ませて描画
		DrawCircleScreen(
			i.pos.x, i.pos.y, // 中心座標
			i.range,    // 内側円のサイズ
			0.f,          // 外側円のサイズ
			0.f,	          // 内側に引き込まれるドット数
			i.sc);
		//DrawEllipseScreen(i.pos.x, i.pos.y, i.range, i.sc);
	}

	if (m_dist.empty())
		DrawGraph(0, 0, GrHandle, TRUE);
}


void Stage::Distortion::PlayDistortion(double x, double y)
{
	tDist tmp;
	tmp.pos.SetVec(x, y);
	tmp.time = 0;
	tmp.range = 0;
	tmp.isPlay = true;
	tmp.sc = MakeScreen(640, 480, TRUE);
	m_dist.emplace_back(tmp);
}
