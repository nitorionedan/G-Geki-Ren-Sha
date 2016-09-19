#include "OpeningStage.hpp"
#include "Graphics2D.hpp"
#include "Counter.hpp"
#include "EnemyMng.hpp"
#include "Keyboard.hpp"
#include <DxLib.h>
#include <cassert>
#include <algorithm>
#include <cmath>

#undef max
#undef min

constexpr int TransTime = 180;
constexpr double Cycle = 0.8;
constexpr double Shake = 70.;


OpenigStage::OpenigStage()
	: SPEED_BACK(1.)
	, SPEED_MIDDLE(3.)
	, SPEED_FRONT(6.)
{
	hg.at(eBG_back) = LoadGraph("GRAPH/GAME/BACKGROUND/RasterOnly/rbg00.png");
	hg.at(eBG_middle) = LoadGraph("GRAPH/GAME/BACKGROUND/sumple_middle.png");
	hg.at(eBG_front) = LoadGraph("GRAPH/GAME/BACKGROUND/RasterOnly/rbg01.png");
	Screen = MakeScreen(640, 480, TRUE);

	Initialize();
}


OpenigStage::~OpenigStage()
{
	for (auto Gr_Handle : hg)
		DeleteGraph(Gr_Handle);
}


void OpenigStage::Initialize()
{
	stage_back.at(0).SetVec(320.0, 240.0);
	stage_back.at(1).SetVec(320.0, -400.0); // pixサイズが例外的に 320 x 320　なので
	stage_mid.at(0).SetVec(320.0, 240.0);
	stage_mid.at(1).SetVec(320.0, -240.0);
	stage_fro.at(0).SetVec(320.0, 240.0);
	stage_fro.at(1).SetVec(320.0, -240.0);
	elapsedTime = 0;
	c_trans = 0;
	cycle = 0.;
	shake = 0.;
}


void OpenigStage::Update()
{
	++elapsedTime;

	// Scroll
	for (auto& back : stage_back)
	{
		back.y += SPEED_BACK;
		back.y = static_cast<double>(WrapPos(static_cast<int>(back.y), 880, -400));
	}

	for (auto& middle : stage_mid)
	{
		middle.y += SPEED_MIDDLE;
		middle.y = static_cast<double>(WrapPos(static_cast<int>(middle.y), 720, -240));
	}

	for (auto& front : stage_fro)
	{
		front.y += SPEED_FRONT;
		front.y = static_cast<double>(WrapPos(static_cast<int>(front.y), 720, -240));
	}

	if (IEnemyMng::IsBossZone() || CheckHitKey(KEY_INPUT_L) != 0)
	{
		if (TransTime > c_trans)
			++c_trans;
	}
	else
	{
		if (0 < c_trans)
			--c_trans;
	}

	cycle = Cycle * (static_cast<double>(c_trans) / TransTime);
	shake = Shake * (static_cast<double>(c_trans) / TransTime);
}


void OpenigStage::Draw()
{
	int tmpSc = GetDrawScreen();
	SetDrawScreen(Screen);
	ClearDrawScreen();

	for (auto& back : stage_back)
		DrawRasterScroll(static_cast<int>(back.x), static_cast<int>(back.y), cycle, shake, elapsedTime, hg[eBG_back], false);
	for (auto& front : stage_fro)
		DrawRasterScroll(static_cast<int>(front.x), static_cast<int>(front.y), cycle, shake, elapsedTime, hg[eBG_front], false);

	SetDrawScreen(DX_SCREEN_BACK);
	DrawGraph(0, 0, Screen, TRUE);
	SetDrawScreen(tmpSc);

	DrawFormatString(320, 40, GetColor(255, 0, 0), "cycle = %lf, shake = %lf", cycle, shake);
}


int OpenigStage::WrapPos(int val, int max, int min)
{
	assert(max > min && "Stage::WrapPos()");

	const int& offset = (val - min) % (max - min);
	return (offset >= 0) ? (offset + min) : (offset + max);
}