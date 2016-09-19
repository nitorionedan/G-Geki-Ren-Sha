#include "OpeningStage.hpp"
#include "Graphics2D.hpp"
#include "Counter.hpp"
#include <DxLib.h>
#include <cassert>
#include <algorithm>
#include <cmath>

#undef max
#undef min


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
}


void OpenigStage::Draw()
{
	int tmpSc = GetDrawScreen();
	SetDrawScreen(Screen);
	ClearDrawScreen();

	for (auto& back : stage_back)
		DrawRotaGraph(static_cast<int>(std::ceil(back.x)), static_cast<int>(std::ceil(back.y)), 1., 0., hg[eBG_back], TRUE);
	for (auto& front : stage_fro)
		DrawRotaGraph(static_cast<int>(std::ceil(front.x)), static_cast<int>(std::ceil(front.y)), 1., 0., hg[eBG_front], TRUE);

	SetDrawScreen(DX_SCREEN_BACK);

//	DrawRasterScroll(320, 240, 0., 0., elapsedTime, Screen, true);
	DrawGraph(0, 0, Screen, TRUE);
	
	SetDrawScreen(tmpSc);
}


int OpenigStage::WrapPos(int val, int max, int min)
{
	assert(max > min && "Stage::WrapPos()");

	const int& offset = (val - min) % (max - min);
	return (offset >= 0) ? (offset + min) : (offset + max);
}