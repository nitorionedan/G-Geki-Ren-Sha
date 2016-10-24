#include "SmokeEffect.hpp"
#include "FileDef.h"
#include "Graphics2D.hpp"
#include <DxLib.h>
#include <algorithm>


namespace
{
	const int Anim_time = 8;
}


SmokeEffect::SmokeEffect()
	: All_time(_countof(gh_green) * Anim_time)
{
	LoadDivGraph(MyFile::Gr::SMOKE00, _countof(gh_green), 4, 2, 20, 20, gh_green);
	m_smoke.reserve(100);
}


SmokeEffect::~SmokeEffect()
{
	for (auto i : gh_green)
		DeleteGraph(i);
}


void SmokeEffect::Update()
{
	Update_Time();
	CheckEndTime();
}


void SmokeEffect::Draw()
{
	for (auto i : m_smoke)
		DrawAnime(i.pos.x, i.pos.y, 2, i.rad, i.time, _countof(gh_green), Anim_time, gh_green);
}


void SmokeEffect::Update_Time()
{
	for (auto& i : m_smoke)
		++i.time;
}


void SmokeEffect::CheckEndTime()
{
	/* If smoke obj is  */
	m_smoke.erase(std::remove_if
		(std::begin(m_smoke), std::end(m_smoke),
		[=](const tSmoke& smoke)->bool {
			return (smoke.time >= All_time);
		}),
		std::end(m_smoke));
}


void SmokeEffect::PlayAnime(const double & x, const double & y, eSmokeColor colorType)
{
	/* set data to tmp */
	tSmoke tmp;
	tmp.pos.SetVec(x, y);
	tmp.isExist = true;
	tmp.colorType = colorType;
	tmp.rad = GetRand(360) * DX_PI / 180;
	tmp.time = 0;

	m_smoke.emplace_back(tmp);	// register
}
