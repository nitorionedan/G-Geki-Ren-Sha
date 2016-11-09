#include "CycloneEffect.hpp"
#include "Player.hpp"
#include "FileDef.h"
#include <DxLib.h>
#include <algorithm>
#include <functional>


namespace
{
	constexpr int    ENERGY_PLAY_TIME = 50;
	constexpr int    PLAY_TIME        = 100;
	constexpr int    CREATE_INTERVAL  = 10;
	constexpr double ADD_RADIAN       = 0.1;
	constexpr double FIRST_DISTANCE   = 350;
	constexpr double DISTANCE_EASING = 0.98;

	/* @brief  Convert degree to radian. */
	auto DegToRad = [](double degAng) {
		return degAng * DX_PI / 180;
	};

	/* @brief  Convert radian to degree. */
	auto RadToDeg = [](double radAng) {
		return radAng * 180 / DX_PI;
	};
}


int CycloneEffect::s_playNum = 0;


CycloneEffect::CycloneEffect()
{
	gh_energy = LoadGraph(MyFile::Gr::FIRE);
	time = 0;
	isPlay = false;
	pos = Vector2D::ZERO;
}


CycloneEffect::~CycloneEffect()
{
	DeleteGraph(gh_energy);
}


void CycloneEffect::Update()
{
	if (isPlay) {
		++time;
	}

	/* time count up */
	for (auto& i : m_tParam) {
		++i.time;
	}
	
	/* Create the cyclone. */
	if (isPlay)
	{
		if (time % CREATE_INTERVAL == 1)
		{
			for (int i = 0; i < 3; ++i) {
				Create(pos.x, pos.y, DegToRad(120 + (i * 120)));
			}
		}

		if (time % CREATE_INTERVAL == 3)
		{
			for (int i = 0; i < 3; ++i) {
				Create(pos.x, pos.y, DegToRad(120 + (i * 120)));
			}
		}
	}

	/* Add the objects radian. */
	std::for_each(std::begin(m_tParam), std::end(m_tParam),
		[](tParam& param) { param.radAng += ADD_RADIAN; });

	/* Shrink the objects. */
	std::for_each(std::begin(m_tParam), std::end(m_tParam),
		[](tParam& param) { param.exrate -= param.dis_exrate; });

	/* Move the objects. */
	//std::for_each(std::begin(m_tParam), std::end(m_tParam),
	//	[](tParam& param) { param.distance -= param.dis_distance; });
	std::for_each(std::begin(m_tParam), std::end(m_tParam),
		[](tParam& param) { param.distance *= DISTANCE_EASING; });

	/* Delete the finised obect. */
	m_tParam.erase(std::remove_if(std::begin(m_tParam), std::end(m_tParam),
		[](const tParam& param)->bool {
			return param.time >= ENERGY_PLAY_TIME;
		}), std::end(m_tParam));

	/* Check time up. */
	if (time >= PLAY_TIME)
	{
		isPlay = false;
		time = 0;
	}
}


void CycloneEffect::Draw()
{
	for (auto i : m_tParam) 
	{
		/* flashing */
		//if (i.time % 2 == 0)
		//	continue;

		DrawRotaGraph(i.pos.x + std::cos(i.radAng) * i.distance,
			i.pos.y + std::sin(i.radAng) * i.distance,
			i.exrate, 0., gh_energy, TRUE);

		/* Follow the players position. */
		//DrawRotaGraph(IPlayer::GetPos().x + std::cos(i.radAng) * i.distance,
		//	IPlayer::GetPos().y + std::sin(i.radAng) * i.distance,
		//	i.exrate, 0., gh_energy, TRUE);
	}
}


void CycloneEffect::PlayAnime(double x, double y) 
{
	if (isPlay) {
		return;
	}

	//++s_playNum;

	isPlay = true;
	pos.SetVec(x, y);
}


void CycloneEffect::Create(double x, double y, double radian)
{
	tParam tmp;
	tmp.time = 0;
	tmp.exrate = 2;
	tmp.distance = FIRST_DISTANCE;
	tmp.radAng = radian;
	tmp.isPlay = true;
	tmp.pos.SetVec(x, y);
	tmp.dis_exrate = tmp.exrate / ENERGY_PLAY_TIME;
	tmp.dis_distance = tmp.distance / ENERGY_PLAY_TIME;

	m_tParam.emplace_back(tmp);
}
