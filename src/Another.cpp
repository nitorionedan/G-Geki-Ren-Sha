#include "DxLib.h"

#include "Another.hpp"
#include "Keyboard.hpp"
#include "DebugMode.hpp"

#include <cmath>

#define MOVING_TIME 1200


Another::Another(ISceneChanger* changer) :
	BaseScene(changer),
	MOVE_TIME(MOVING_TIME),
	testPTCL(new Particle(300)),
	c_start(new Counter(180)),
	c_candle(new Counter(MOVING_TIME)),
	FIRST_POS_X(320.0),
	FIRST_POS_Y(240.0),
	FIRST_EXRATE(1.0)
{
	gh_myface = LoadGraph("GRAPH/HOHO/myface.png");
	gh_ptcl   = LoadGraph("GRAPH/PARTICLEs/ptcl00.png");
	gh_candle = LoadGraph("GRAPH/HOHO/rou.png");
	gh_doll   = LoadGraph("GRAPH/HOHO/doll_3.png");
	sh_bgm    = LoadSoundMem("SOUND/MIMI/shingyo2.mp3");
	sh_namu   = LoadSoundMem("SOUND/MIMI/namu.wav");
	x_candle.fill(FIRST_POS_X);
	y_candle.fill(FIRST_POS_Y);
	ert_candle.fill(FIRST_EXRATE);

	x_tgt.at(0) = 130.0;
	x_tgt.at(1) = 100.0;
	x_tgt.at(2) = 50.0;

	x_tgt.at(3) = 510.0;
	x_tgt.at(4) = 540.0;
	x_tgt.at(5) = 590.0;

	y_tgt.at(0) = 240.0;
	y_tgt.at(1) = 260.0;
	y_tgt.at(2) = 300.0;

	ert_tgt.at(0) = 0.5;
	ert_tgt.at(1) = 0.7;
	ert_tgt.at(2) = 1.0;

	isMove  = true;
	isPause = false;
	isStart = false;

	PlayBGM();
	SetMouseDispFlag(true);

	// TEST
	test_c = 0;
}


Another::~Another()
{
	DeleteGraph(gh_myface);
	DeleteGraph(gh_ptcl);
}


void Another::Update() 
{
	if (Keyboard_Get(KEY_INPUT_Q) == 1)	isPause = !isPause;

	if (isPause)	return;
	test_c++;

	c_start->Update();
	if (c_start->isLast())	isStart = true;
	if (!isStart) return;

	c_candle->Update();

	if (isMove)
	{
		for (int I = 0; I != x_candle.size(); ++I)
			x_candle.at(I) += ( (FIRST_POS_X - x_tgt.at(I)) / MOVE_TIME );		// ろうそくの動きX

		for (int I = 0; I != y_candle.size(); ++I)
			y_candle.at(I) += ( (y_tgt.at(I) - FIRST_POS_Y) / MOVE_TIME );		// ろうそくの動きY

		for (int I = 0; I != ert_candle.size(); ++I)
			ert_candle.at(I) += ( (ert_tgt.at(I) - FIRST_EXRATE) / MOVE_TIME );	// ろうそくの倍率
	}

	if (c_candle->isLast())
	{
		c_candle->isCount(false);
		isMove = false;
	}

	// TEST -------------------------------------------------------------------------------
	if (!DebugMode::isTest)	return;

	testPTCL->Update();
}


void Another::Draw() 
{
	SetDrawBlendMode(DX_BLENDMODE_ADD, 30);			// 加算ブレンドモード
	Raster_Scroll(320, 240, gh_myface, 0.83, 4.6);	// ラスタースクロール
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// ブレンドモードをオフ

	DrawBlendAdd(600, 400, 3.0, 0.0, 20, gh_doll);	// 透明人形

	for (int I = 0; I != x_candle.size(); ++I)
		DrawRotaGraph(x_candle.at(I), y_candle.at(I % 3), ert_candle.at(I % 3), 0.0, gh_candle, true, false);

	// TEST -------------------------------------------------------------------------------
	if (!DebugMode::isTest)	return;

	testPTCL->Draw(gh_ptcl);
	DrawFormatString(0, 20, GetColor(0, 255, 0), "counter = %d", c_candle->GetNowcount());
	DrawFormatString(0, 50, GetColor(0, 255, 0), "test    = %d", test_c);
}


void Another::PlayBGM()
{
	PlaySoundMem(sh_bgm, DX_PLAYTYPE_LOOP);		// 約1650fps でお経
	PlaySoundMem(sh_namu, DX_PLAYTYPE_BACK);
}