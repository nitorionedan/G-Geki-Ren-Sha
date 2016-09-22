#include "OpeningStage.hpp"
#include "Graphics2D.hpp"
#include "Counter.hpp"
#include "EnemyMng.hpp"
#include "Keyboard.hpp"
#include <DxLib.h>
#include <cassert>
#include <algorithm>
#include <cmath>
#include <functional>

#undef max
#undef min

constexpr int TransTime = 180;
constexpr int CenterX = 320;
constexpr int CenterY = 240;
constexpr double Cycle = 0.8;
constexpr double Shake = 70.;
constexpr double Harf = 0.5;

namespace
{
	std::function<double(double val, double max, double min)> Wrap = 
		[](double val, double max, double min)
		{
			assert(max > min && "Stage::WrapPos()");
			const double& offset = std::fmod((val - min), (max - min));
			return (offset >= 0) ? (offset + min) : (offset + max);
		};
}


OpenigStage::OpenigStage()
	: SPEED_BACK(1.)
	, SPEED_MIDDLE(3.)
	, SPEED_FRONT(6.)
{
	hg.at(eBG_back) = LoadGraph("GRAPH/GAME/BACKGROUND/RasterOnly/rbg00.png");
	hg.at(eBG_middle) = LoadGraph("GRAPH/GAME/BACKGROUND/sumple_middle.png");
	hg.at(eBG_front) = LoadGraph("GRAPH/GAME/BACKGROUND/RasterOnly/rbg01.png");
	Screen = MakeScreen(640, 480, TRUE);

	testGh = LoadGraph("GRAPH/GAME/BACKGROUND/moon.png");

	Initialize();
}


OpenigStage::~OpenigStage()
{
	DeleteGraph(testGh);

	for (auto Gr_Handle : hg)
		DeleteGraph(Gr_Handle);
}


void OpenigStage::Initialize()
{
	int GrSizeX, GrSizeY;
	GetGraphSize(hg.at(eBG_back), &GrSizeX, &GrSizeY);
	GrSizeX /= 2;
	GrSizeY /= 2;
	for (int i = 0; i != stage_back.size(); ++i)
		stage_back[i].SetVec(CenterX, CenterY - (i * GrSizeY * 2));

	GetGraphSize(hg.at(eBG_front), &GrSizeX, &GrSizeY);
	GrSizeX /= 2;
	GrSizeY /= 2;
	for (int i = 0; i != stage_fro.size(); ++i)
		stage_fro[i].SetVec(CenterX, CenterY - (i * GrSizeY * 2));

	elapsedTime = 0;
	c_trans = 0;
	cycle = Cycle;
	shake = Shake;
}


void OpenigStage::Update()
{
	++elapsedTime;

	double bi = 1.;
	if (IEnemyMng::IsBossZone())
	{
		if (255 > c_trans)
			++c_trans;
		bi = 0.5;
	}
	else
	{
		if (0 < c_trans)
			--c_trans;
	}

	// Scroll
	for (auto& back : stage_back)
	{
		back.y += SPEED_BACK * bi;
		//back.y = Wrap(back.y, 880, -400);
		back.y = Wrap(back.y, 800, -480);
	}

	for (auto& front : stage_fro)
	{
		front.y += SPEED_FRONT * bi;
		//front.y = Wrap(front.y, 480 + (120 * 2), -120 * 2);
		front.y = Wrap(front.y, 720 + (240), -480 + (240));
	}
}


void OpenigStage::Draw()
{
	int tmpSc = GetDrawScreen();
	SetDrawScreen(Screen);
	ClearDrawScreen();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, c_trans);
	SetDrawBright(255, 100, 100);
	for (auto& back : stage_back)
		DrawRasterScroll(static_cast<int>(back.x), static_cast<int>(back.y), cycle, shake, elapsedTime, hg[eBG_back], false);
	for (auto& front : stage_fro)
		DrawRasterScroll(static_cast<int>(front.x), static_cast<int>(front.y), cycle, shake, elapsedTime, hg[eBG_front], false);
	SetDrawBright(255, 255, 255);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - c_trans);
	for (auto& back : stage_back)
		DrawRotaGraphF(static_cast<float>(back.x), static_cast<float>(back.y), 1., 0., hg[eBG_back], TRUE);
	for (auto& front : stage_fro)
		DrawRotaGraphF(static_cast<float>(front.x), static_cast<float>(front.y), 1., 0., hg[eBG_front], TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetDrawScreen(DX_SCREEN_BACK);
	DrawGraph(0, 0, Screen, TRUE);
	SetDrawScreen(tmpSc);
}


int OpenigStage::WrapPos(int val, int max, int min)
{
	assert(max > min && "Stage::WrapPos()");

	const int& offset = (val - min) % (max - min);
	return (offset >= 0) ? (offset + min) : (offset + max);
}