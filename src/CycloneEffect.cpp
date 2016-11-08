#include "CycloneEffect.hpp"
#include "Player.hpp"
#include "FileDef.h"
#include <DxLib.h>
#include <algorithm>
#include <functional>


namespace
{
	constexpr int PLAY_TIME = 100;
	constexpr double ADD_RADIAN = 0.2;
	constexpr double FIRST_DISTANCE = 140;

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
}


CycloneEffect::~CycloneEffect()
{
	DeleteGraph(gh_energy);
}


void CycloneEffect::Update()
{
	/* time count up */
	for (auto& i : m_tParam) {
		++i.time;
	}
	
	/* Add the objects radian. */
	std::for_each(std::begin(m_tParam), std::end(m_tParam),
		[](tParam& param) { param.radAng += ADD_RADIAN; });

	/* Shrink the objects. */
	std::for_each(std::begin(m_tParam), std::end(m_tParam),
		[](tParam& param) { param.exrate -= param.dis_exrate; });

	/* Move the objects. */
	std::for_each(std::begin(m_tParam), std::end(m_tParam),
		[](tParam& param) { param.distance -= param.dis_distance; });

	/* Delete the finised obect. */
	m_tParam.erase(std::remove_if(std::begin(m_tParam), std::end(m_tParam),
		[](const tParam& param)->bool {
			return param.time >= PLAY_TIME;
		}),
	std::end(m_tParam));
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


void CycloneEffect::PlayAnime(double x, double y) {
	++s_playNum;
	Create(x, y, DegToRad(90));
}


void CycloneEffect::Create(double x, double y, double radian)
{
	tParam tmp;
	tmp.time = 0;
	tmp.exrate = 2;
	tmp.distance = FIRST_DISTANCE;
	tmp.radAng = 0;
	tmp.isPlay = true;
	tmp.pos.SetVec(x, y);
	tmp.dis_exrate = tmp.exrate / PLAY_TIME;
	tmp.dis_distance = tmp.distance / PLAY_TIME;

	m_tParam.emplace_back(tmp);
}
