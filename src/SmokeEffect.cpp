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
	LoadDivGraph(GR_FILE_SMOKE00, _countof(gh_green), 4, 2, 20, 20, gh_green);
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

	if (CheckHitKey(KEY_INPUT_V) != 0)
		m_smoke.erase(std::begin(m_smoke), std::end(m_smoke));

	clsDx();
	printfDx("%d", m_smoke.size());
} // F end


void SmokeEffect::Draw()
{
	for (auto i : m_smoke)
	{
		//if (i.time >= 1 && i.time <= 4)
		//	DrawRotaGraph(i.pos.x, i.pos.y, 2, i.rad, gh_green[0], TRUE);
		//if (i.time >= 3 && i.time <= 4)
		//	DrawRotaGraph(i.pos.x, i.pos.y, 2, i.rad, gh_green[1], TRUE);
		//if (i.time >= 5 && i.time <= 6)
		//	DrawRotaGraph(i.pos.x, i.pos.y, 2, i.rad, gh_green[2], TRUE);
		//if (i.time >= 7 && i.time <= 8)
		//	DrawRotaGraph(i.pos.x, i.pos.y, 2, i.rad, gh_green[3], TRUE);
		//if (i.time >= 9 && i.time <= 10)
		//	DrawRotaGraph(i.pos.x, i.pos.y, 2, i.rad, gh_green[4], TRUE);
		//if (i.time >= 11 && i.time <= 12)
		//	DrawRotaGraph(i.pos.x, i.pos.y, 2, i.rad, gh_green[5], TRUE);
		//if (i.time >= 13 && i.time <= 16)
		//	DrawRotaGraph(i.pos.x, i.pos.y, 2, i.rad, gh_green[6], TRUE);
		//if (i.time >= 17 && i.time <= 20)
		//	DrawRotaGraph(i.pos.x, i.pos.y, 2, i.rad, gh_green[7], TRUE);

		//for (int id = 0; id < _countof(gh_green), ++id)
		//{
		//	if (i.time >= 1 + (id * (All_time / _countof(gh_green))) && i.time <= )
		//}

		DrawAnime(i.pos.x, i.pos.y, 2, i.rad, i.time, _countof(gh_green), Anim_time, gh_green);
	}
}


void SmokeEffect::Update_Time()
{
	for (auto& i : m_smoke)
		++i.time;
}


void SmokeEffect::CheckEndTime()
{
	m_smoke.erase(std::remove_if
		(std::begin(m_smoke), std::end(m_smoke),
		[=](const tSmoke& smoke)->bool {
			return (smoke.time >= All_time);
		}),
		std::end(m_smoke));
}


void SmokeEffect::PlayAnime(const double & x, const double & y, eSmokeColor colorType)
{
	tSmoke tmp;
	tmp.pos.SetVec(x, y);
	tmp.isExist = true;
	tmp.colorType = colorType;
	tmp.rad = GetRand(360) * DX_PI / 180;
	tmp.time = 0;

	m_smoke.emplace_back(tmp);
}
