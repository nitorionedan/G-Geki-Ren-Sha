/*
@brief  Define of EneShot.hpp
@author Shohei
*/
#include "EneShot.hpp"
#include "EneShotAI.hpp"
#include "Graphics2D.hpp"
#include "Player.hpp"
#include "HitEffect.hpp"
#include "DebugMode.hpp"
#include <DxLib.h>
#include <cassert>
#include <algorithm>


namespace
{
	constexpr double BD_LEFT = -10.;
	constexpr double BD_RIGHT = 660.;
	constexpr double BD_TOP = BD_LEFT;
	constexpr double BD_BOTTOM = 500.;
};


EneShot::EneShot()
{
	LoadDivGraph("GRAPH/GAME/Eshot/eshot00.png", 12, 4, 3, 8, 8, gh_normal);
	LoadDivGraph("GRAPH/GAME/Eshot/eshot02.png", 24, 8, 3, 9, 33, gh_wave);
	gh_long[0] = LoadGraph("GRAPH/GAME/Eshot/enaga0.png");
	gh_long[1] = LoadGraph("GRAPH/GAME/Eshot/enaga1.png");
	gh_long[2] = LoadGraph("GRAPH/GAME/Eshot/enaga2.png");
	gh_long[3] = LoadGraph("GRAPH/GAME/Eshot/enaga3.png");
	gh_long[4] = LoadGraph("GRAPH/GAME/Eshot/enaga4.png");
	LoadDivGraph("GRAPH/GAME/Eshot/bigshot.png", 4, 4, 1, 60, 60, gh_big_O);
	LoadDivGraph("GRAPH/GAME/Eshot/eshot04.png", 10, 5, 2, 13, 12, gh_star);

	shot.reserve(100);
}


EneShot::~EneShot()
{
	for (auto i : gh_normal)
		DeleteGraph(i);
	for (auto i : gh_wave)
		DeleteGraph(i);
	for (auto i : gh_long)
		DeleteGraph(i);
	for (auto i : gh_big_O)
		DeleteGraph(i);
	for (auto i : gh_star)
		DeleteGraph(i);
}


void EneShot::Update()
{
	/* time count */
	for (auto& i : shot)
		++i.time;

	/* move */
	for (auto& i : shot)
		i.mAI->Update(i);

	/* erase */
	for (auto itr = std::begin(shot); itr != std::end(shot); ++itr)
	{
		if ((*itr).pos.x < BD_LEFT || (*itr).pos.x > BD_RIGHT  ||
			(*itr).pos.y < BD_TOP  || (*itr).pos.y > BD_BOTTOM)
		{
			delete (*itr).mAI;
			shot.erase(itr);
			break;
		}
	}

	/* col check */
	HitCheck();
}


void EneShot::Draw()
{
	for (auto i : shot)
	{
		switch (i.shotType)
		{
		case eShotType::normal:
			DrawAnime(i.pos.x, i.pos.y, 2., i.force.ToRad(), i.time,
				_countof(gh_normal), 2, gh_normal);
			break;
		case eShotType::star:
			DrawAnime(i.pos.x, i.pos.y, 2., i.force.ToRad(), i.time,
				_countof(gh_star), 2, gh_star);
			break;
		case eShotType::wave:
			DrawAnime(i.pos.x, i.pos.y, 2., -i.force.ToRad(), i.time,
				_countof(gh_wave), 2, gh_wave);
			break;
		case eShotType::big_O:
			DrawAnime(i.pos.x, i.pos.y, 2., i.force.ToRad(), i.time,
				_countof(gh_big_O), 2, gh_big_O);
			break;
		case eShotType::laser:
			// TODO: 
			break;
		case eShotType::longer:
			DrawAnime(i.pos.x, i.pos.y, 2., i.force.ToRad(), i.time,
				_countof(gh_long), 2, gh_long);
			break;

		default: assert(!"abnormality val");
		}
	}

	if ( DebugMode::isTest )
	{
		for (auto i : shot)
			DrawCircle(i.pos.x, i.pos.y, i.hitRange, GetColor(0, 255, 0), TRUE);
	}
}


void EneShot::Fire(eShotType type, Vector2D & pos, double rotate, Vector2D & force, double accel, int life, eShotAI aiType)
{
	tShot tmp;
	tmp.shotType = type;
	tmp.pos = pos;
	tmp.force = force;
	tmp.accel = accel;
	tmp.life = life;
	tmp.time = 0;
	tmp.rad = 0;
	SetRange(type, tmp.hitRange);
	switch (aiType)
	{
	case eShotAI::straight:
		tmp.mAI = static_cast<EneShotAI*>(new EneShotAI_Straight);
		break;
	case eShotAI::outsideCurve:
		tmp.mAI = static_cast<EneShotAI*>(new EneShotAI_OutsideCurve);
		break;
	case eShotAI::insideCurve:
		tmp.mAI = static_cast<EneShotAI*>(new EneShotAI_InsideCurve);
		break;
	case eShotAI::wave:
		tmp.mAI = static_cast<EneShotAI*>(new EneShotAI_Wave);
		break;
	}

	shot.emplace_back(tmp);
}


void EneShot::Fire_Ang(eShotType type, Vector2D & pos, double rotate, double force, double angle, double accel, int life, eShotAI aiType)
{
	tShot tmp;
	tmp.shotType = type;
	tmp.pos = pos;
	tmp.force.x = force * std::cos(angle);
	tmp.force.y = force * std::sin(angle);
	tmp.accel = accel;
	tmp.life = life;
	tmp.time = 0;
	tmp.rad = 0;
	SetRange(type, tmp.hitRange);
	switch (aiType)
	{
	case eShotAI::straight:
		tmp.mAI = static_cast<EneShotAI*>(new EneShotAI_Straight);
		break;
	case eShotAI::outsideCurve:
		tmp.mAI = static_cast<EneShotAI*>(new EneShotAI_OutsideCurve);
		break;
	case eShotAI::insideCurve:
		tmp.mAI = static_cast<EneShotAI*>(new EneShotAI_InsideCurve);
		break;
	case eShotAI::wave:
		tmp.mAI = static_cast<EneShotAI*>(new EneShotAI_Wave);
		break;
	}
	shot.emplace_back(tmp);
}


void EneShot::HitCheck()
{
	/* col Player */
	for (auto itr = std::begin(shot); itr != std::end(shot); ++itr)
	{
		if (IPlayer::HitCheckCircl( (*itr).hitRange, (*itr).pos) == false)
			continue;
		IHitEffect::PlayAnime( (*itr).pos );
		delete (*itr).mAI;
		shot.erase(itr);
		break;
	}

	/* col Player's shot */
	for (auto& i : shot)
	{
		if (i.life == 0)
			continue;

		// TODO: HitCheck
		delete i.mAI;
		// erase
		break;
	}
}

void EneShot::SetRange(eShotType type, double & hitRange) const
{
	/* range of hit */
	switch (type)
	{
	case eShotType::normal:
		hitRange = 7;
		break;
	case eShotType::star:
		hitRange = 7;
		break;
	case eShotType::wave:
		hitRange = 7;
		break;
	case eShotType::big_O:
		hitRange = 50;
		break;
	case eShotType::laser:
		break;
	case eShotType::longer:
		break;
	case eShotType::missile:
		break;
	}
}

// Interface============================================================================
std::shared_ptr<EneShot> IEneShot::mEneShot;


void IEneShot::set(std::shared_ptr<EneShot> eneShot) {
	mEneShot = eneShot;
}


void IEneShot::reset() {
	mEneShot.reset();
}


tShot::tShot(EneShotAI * ai) {
	this->mAI = ai;
}
