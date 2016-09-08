#include "EneShotCreater.hpp"
#include "Graphics2D.hpp"
#include "Player.hpp"
#include "HitEffect.hpp"
#include "Bomb.hpp"

// move
#include "EneShotStraight.hpp"
#include "EneShotSniper.hpp"

#include <DxLib.h>
#include <cmath>
#include <algorithm>
#include <cassert>

const double EneShotCreater::BD_LEFT = -10.;
const double EneShotCreater::BD_RIGHT = 660.;
const double EneShotCreater::BD_TOP = BD_LEFT;
const double EneShotCreater::BD_BOTTOM = 500.;


EneShotCreater::EneShotCreater()
{
	LoadDivGraph("GRAPH/GAME/Eshot/eshot00.png", 12, 4, 3, 8, 8, gh_ball);
	LoadDivGraph("GRAPH/GAME/Eshot/eshot02.png", 24, 8, 3, 9, 33, gh_power);
	gh_long[0] = LoadGraph("GRAPH/GAME/Eshot/enaga0.png");
	gh_long[1] = LoadGraph("GRAPH/GAME/Eshot/enaga1.png");
	gh_long[2] = LoadGraph("GRAPH/GAME/Eshot/enaga2.png");
	gh_long[3] = LoadGraph("GRAPH/GAME/Eshot/enaga3.png");
	gh_long[4] = LoadGraph("GRAPH/GAME/Eshot/enaga4.png");
	LoadDivGraph("GRAPH/GAME/Eshot/bigshot.png", 4, 4, 1, 60, 60, gh_big);
	LoadDivGraph("GRAPH/GAME/Eshot/eshot04.png", 10, 5, 2, 13, 12, gh_suriken);

	mEneShot.reserve(100);
}


EneShotCreater::~EneShotCreater()
{
	for (auto i : gh_ball)
		DeleteGraph(i);
	for (auto i : gh_power)
		DeleteGraph(i);
	for (auto i : gh_long)
		DeleteGraph(i);
	for (auto i : gh_big)
		DeleteGraph(i);
	for (auto i : gh_suriken)
		DeleteGraph(i);
}


void EneShotCreater::Update()
{
	for(auto i : mEneShot)
	{
		i->Update(this);

		const bool& Is_hit = i->HitCheck();
		const bool& Is_OB = (i->pos.x < BD_LEFT || i->pos.x > BD_RIGHT ||
							 i->pos.y < BD_TOP || i->pos.y > BD_BOTTOM);

		if (Is_hit == false && Is_OB == false) continue;

		// if shot is OB, then it's removed
		mEneShot.erase(std::remove(std::begin(mEneShot), std::end(mEneShot), i), std::end(mEneShot));
		break;
	}
}


void EneShotCreater::Draw()
{
	for (auto i : mEneShot)
		i->Draw(this);

	DrawFormatString(200, 200, GetColor(0, 255, 0), "size : %d", mEneShot.size());
}


void EneShotCreater::Fire(eEneShot shotType, eEneShotMove moveType, Vector2D& pos, double& speed)
{
	mEneShot.emplace_back(new EneShot(shotType, moveType, pos, speed));
}


// EneShot(Inner class)==============================================================

EneShotCreater::EneShot::EneShot(eEneShot shotType, eEneShotMove moveType, Vector2D firstPos, double speed)
{
	hitRange = 1.;

	this->shotType = shotType;
	switch (moveType)
	{
	case eEneShotMove::straight:
		mMove = (EneShotMove*)(new EneShotStraight);
		hitRange = 7.;
		break;
	case eEneShotMove::follow:
		break;
	case eEneShotMove::accel:
		break;
	case eEneShotMove::sniper: mMove = (EneShotMove*)(new EneShotSniper);
	default: assert(!"EneShotCreater::EneShot::EneShot()");
	}
	pos = firstPos;
	this->speed = speed;

	Initialize();
}


EneShotCreater::EneShot::~EneShot()
{
	delete mMove;
}


void EneShotCreater::EneShot::Update(EneShotCreater * pEneShotCreater)
{
	elapsedTime++;

	mMove->Move(this);

	HitCheck();
}


void EneShotCreater::EneShot::Draw(EneShotCreater* pEShotCreater)
{
	switch (shotType)
	{
	case eEneShot::power:
		break;
	case eEneShot::ball:
		DrawAnime(pos.x, pos.y, 2., angle, elapsedTime,
		_countof(pEShotCreater->gh_ball), 2, pEShotCreater->gh_ball);
		break;
	case eEneShot::longer:
		break;
	case eEneShot::big:
		break;
	case eEneShot::suriken:
		break;
	default: assert(!"EneShotCreater::EneShot::Draw()");
	}
}


bool EneShotCreater::EneShot::HitCheck()
{
	const bool& Is_hit = IPlayer::HitCheckCircl(hitRange, pos) |
						 IBomb::IsHit(hitRange, pos.x, pos.y);
	
	if (Is_hit)	IHitEffect::PlayAnime(pos);
	
	return Is_hit;
}


void EneShotCreater::EneShot::Initialize()
{
	angle = 0.;
	elapsedTime = 0;
}


// ===========================================================
std::shared_ptr<EneShotCreater> IEneShotCreater::mEShotCreater;


void IEneShotCreater::set(std::shared_ptr<EneShotCreater> eneShotCreater)
{
	mEShotCreater = eneShotCreater;
}


void IEneShotCreater::Fire(eEneShot shotType, eEneShotMove moveType, Vector2D & firstPos, double & speed)
{
	mEShotCreater->Fire(shotType, moveType, firstPos, speed);
}
