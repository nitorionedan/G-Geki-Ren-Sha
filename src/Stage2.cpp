#include "Stage2.hpp"
#include "Sound.hpp"
#include "Stage.hpp"
#include <DxLib.h>
#include <functional>
#include <cassert>
#include <cmath>

namespace
{
	constexpr int POS_MAX =  720.;
	constexpr int POS_MIN = -240.;
	constexpr int MOON_START_TIME = 1831473;
	constexpr int FADEIN_TIME_MOON = 200;
	constexpr int FADEOUT_TIME_CLOUD = 300;
	constexpr double MOON_FIRST_POS_Y = -10;
	constexpr double MOON_END_POS_Y = 240;
	constexpr double ALPHA_FORE_AND_BACK = 130;
	constexpr double ALPHA_MID = 80;

	auto Wrap = [](double val, double max, double min)
	{
		assert(max > min && "Stage2::Wrap");
		const double& offset = std::fmod((val - min), (max - min));
		return (offset >= 0) ? (offset + min) : (offset + max);
	};
}


Stage2::Stage2()
	: ForeSpeed(4.)
	, BackSpeed(2.)
	, MidSpeed(300.)
	, img(new Image)
{
	img->Load("GRAPH/GAME/BACKGROUND/stage2_02.png", "back");
	img->Load("GRAPH/GAME/BACKGROUND/stage2_01.png", "fore");
	img->Load("GRAPH/GAME/BACKGROUND/moon.png", "moon");
	//img->Load("GRAPH/GAME/BACKGROUND/stage2_01.png", "back");

	Initialize();
}


Stage2::~Stage2()
{
}


void Stage2::Update()
{
	c_mid += 0.001;

	if (IStage::GetTime() == 2500 - 150) // -100 good enough
		isMoonTime = true;

	for (auto& pos : back_pos)
	{
		pos.y += BackSpeed;
		pos.y = Wrap(pos.y, POS_MAX, POS_MIN);
	}

	for (auto& pos : mid_pos)
	{
		pos.y = std::sin(c_mid) * MidSpeed;
	}

	for (auto& pos : fore_pos)
	{
		pos.y += ForeSpeed;
		pos.y = Wrap(pos.y, POS_MAX, POS_MIN);
	}

	Update_moon();

	//DrawFormatString(320, 240, GetColor(0, 255, 0), "C_moon=%lf", c_moon);
	//DrawFormatString(320, 260, GetColor(0, 255, 0), "time=%d", IStage::GetTime());
}


void Stage2::Draw()
{
	/* なんとなく　イルミネーション */
	//DrawBox(0, 0, 640, 480, GetColor(c_moon, 0, c_moon), TRUE);

	SetDrawMode(DX_DRAWMODE_BILINEAR);

	// Middle
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, c_alpha_mid);
	//SetDrawBlendMode(DX_BLENDMODE_ADD, c_cloud_bright);
	for (auto pos : mid_pos)
		img->DrawRota(pos.x, pos.y, 4., 0., "back", true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// Moon
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, c_moon);
	img->DrawRota(moon_pos.x, moon_pos.y, 2., 0., "moon", true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// Back
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, c_alpha_back);
	//SetDrawBlendMode(DX_BLENDMODE_ADD, c_cloud_bright);
	for (auto pos : back_pos)
		img->DrawRota(pos.x, pos.y, 2., 0., "fore", true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// Fore
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, c_alpha_fore);
	//SetDrawBlendMode(DX_BLENDMODE_ADD, c_cloud_bright);
	for (auto pos : fore_pos)
		img->DrawRota(pos.x, pos.y, 2., 0., "fore", true, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetDrawMode(DX_DRAWMODE_NEAREST);
}


void Stage2::Initialize()
{
	/* first positions */
	for (auto& pos : back_pos)
		pos.x = 320.;
	for (auto& pos : mid_pos)
		pos.x = 320.;
	for (auto& pos : fore_pos)
		pos.x = 320.;
	fore_pos[0].y = -240.;
	fore_pos[1].y =  240.;
	mid_pos[0].y  =  300.;
	mid_pos[1].y  = -240.;
	back_pos[0].y = -240.;
	back_pos[1].y =  240.;
	moon_pos.SetVec(320, -200);

	/* flags */
	isMoonTime = false;

	/* counters */
	c_moon         = 0.;
	c_mid          = 0.;
	c_cloud_bright = 255.;
	c_alpha_fore   = ALPHA_FORE_AND_BACK;
	c_alpha_mid    = ALPHA_MID;
	c_alpha_back   = ALPHA_FORE_AND_BACK;
}


void Stage2::Update_moon()
{
	if (isMoonTime)
	{
		if (c_moon != 255)
			c_moon += 255 / FADEIN_TIME_MOON;

		/* fore cloud fadeouting */
		if (c_alpha_fore > 0)
			c_alpha_fore -= ALPHA_FORE_AND_BACK / FADEOUT_TIME_CLOUD;

		/* middle cloud fadeouting */
		if (c_alpha_mid > 0)
			c_alpha_mid -= ALPHA_MID / FADEOUT_TIME_CLOUD;

		/* back cloud fadeouting */
		if (c_alpha_back > 0)
			c_alpha_back -= ALPHA_FORE_AND_BACK / FADEOUT_TIME_CLOUD;

		// easing
		moon_pos.y += (MOON_END_POS_Y - moon_pos.y) * 0.01;

		// brightness
		++c_cloud_bright;
		if (c_cloud_bright > 255)
			c_cloud_bright = 255;
	}
}
