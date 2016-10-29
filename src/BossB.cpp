#include "BossB.hpp"
#include "FileDef.h"
#include "EneShot.hpp"
#include "Effector.hpp"
#include "PieceEffect.hpp"
#include "ExplosionEffect.hpp"
#include "IScore.hpp"
#include <DxLib.h>
#include <cmath>

namespace
{
	constexpr int MAX_HP = 4000;
	constexpr int START_ALL_TIME  = 3000;
	constexpr int NORMAL_ALL_TIME = 1800;
	constexpr int STAY_ALL_TIME   = 240;
	constexpr int WEAK_ALL_TIME = 300;
	constexpr int DEAD_ALL_TIME = 300;

	constexpr double START_SPEED = 0.7;
}


BossB::BossB()
	: body_head(new Body(ePart::head))
	, body_leftWing(new Body(ePart::leftWing))
	, body_rightWing(new Body(ePart::rightWing))
	, body_tail00(new Body(ePart::tail00))
	, body_tail01(new Body(ePart::tail01))
	, body_gun(new Body(ePart::gun))
	, img(new Image)
{
	img->Load(MyFile::Gr::BOSS_FLYER_BACK, "back");
	img->Load(MyFile::Gr::BOSS_FLYER_HEAD, "head");
	img->Load(MyFile::Gr::BOSS_FLYER_WING_BACK, "wing_back");
	img->Load(MyFile::Gr::BOSS_FLYER_WING_HEAD, "wing_head");
	img->Load(MyFile::Gr::BOSS_FLYER_TAIL00, "tail00");
	img->Load(MyFile::Gr::BOSS_FLYER_TAIL01, "tail01");
	img->Load(MyFile::Gr::BOSS_FLYER_GUN, "gun");
	img->Load(MyFile::Gr::BOSS_FLYER_L_WING, "Lwing");

	body_head->pos.SetVec(320, -100);
	body_leftWing->pos = body_head->pos;
	body_rightWing->pos = body_head->pos;
	body_tail00->pos = Vector2D::GetVec(body_head->pos.x + 3, body_head->pos.y);
	body_tail01->pos = Vector2D::GetVec(body_head->pos.x + 3, body_head->pos.y);
	body_gun->pos = Vector2D::GetVec(body_head->pos.x + 3, body_head->pos.y);

	hp = MAX_HP;
	elapsedTime = 0;
	c_start = 0;
	c_normal = 0;
	c_stay = 0;
	c_weak = 0;
	c_dead = 0;
	readyWing = false;
	isExist = true;
	isDead = false;

	state = eState::start;
}


BossB::~BossB()
{
}


void BossB::Update()
{
	++elapsedTime;

	switch (state)
	{
	case BossB::eState::start: Update_Start();  break;
	case BossB::eState::normal:Update_Normal();	break;
	case BossB::eState::stay: Update_Stay();    break;
	case BossB::eState::weak: Update_Weak();    break;
	case BossB::eState::dead: Update_Dead();    break;
	}
}


void BossB::Draw()
{
	if (isExist == false)
		return;

	switch (state)
	{
	case BossB::eState::start:
		if (readyWing)
		{
			//img->DrawRota(pos_backWing.x, pos_backWing.y, 2, 0, "wing_head");
			body_leftWing->Draw(*this);
			body_rightWing->Draw(*this);
			body_head->Draw(*this);
			body_tail00->Draw(*this);
			body_tail01->Draw(*this);
		}
		else
			img->DrawRota(pos_backWing.x, pos_backWing.y, 2, 0, "wing_back");
		img->DrawRota(body_head->pos.x, body_head->pos.y, 2, 0, "back");
		break;
	case BossB::eState::normal:
		//img->DrawRota(pos_backWing.x, pos_backWing.y, 2, 0, "wing_head");
		//img->DrawRota(pos_tail01.x, pos_tail01.y, 2, 0, "tail01");
		//img->DrawRota(pos_tail00.x, pos_tail00.y, 2, 0, "tail00");
		//img->DrawRota(pos.x, pos.y, 2, 0, "back");
		break;
	case BossB::eState::stay:
		//body_head->Draw(pos, *this);
		//body_leftWing->Draw(pos, *this);
		//body_rightWing->Draw(pos, *this);
		//body_tail00->Draw(pos, *this);
		//body_tail01->Draw(pos, *this);
		break;
	case BossB::eState::weak:
		//body_head->Draw(pos, *this);
		//body_leftWing->Draw(pos, *this);
		//body_rightWing->Draw(pos, *this);
		//body_tail00->Draw(pos, *this);
		//body_tail01->Draw(pos, *this);
		break;
	case BossB::eState::dead:
		//body_head->Draw();
		//body_leftWing->Draw();
		//body_rightWing->Draw();
		//body_tail00->Draw();
		//body_tail01->Draw();
		break;
	}
}

const Vector2D & BossB::GetPos() const {
	return body_head->pos;
}


bool BossB::HitCheck(const double & ColX, const double & ColY, const int & DamagePoint)
{
	bool isHit;

	if (body_head->HitCheck(ColX, ColY, DamagePoint))
		return true;

	if (body_leftWing->HitCheck(ColX, ColY, DamagePoint))
		return true;

	if (body_rightWing->HitCheck(ColX, ColY, DamagePoint))
		return true;

	if (body_tail00->HitCheck(ColX, ColY, DamagePoint))
		return true;

	if (body_tail01->HitCheck(ColX, ColY, DamagePoint))
		return true;

	if (body_gun->HitCheck(ColX, ColY, DamagePoint))
		return true;

	return false;
}


const bool BossB::IsDead() const {
	return isDead;
}


void BossB::Update_Start()
{
	/* Time count */
	++c_start;
	if (c_start >= START_ALL_TIME)
	{
		c_start = 0;
		state = eState::normal;
	}

	/* Move */
	if (body_head->pos.y < 90.)
	{	
		body_head->pos.y   += START_SPEED;
		body_tail00->pos.y += START_SPEED;
		body_tail01->pos.y += START_SPEED;
	}
	else
	{
		/* back wing start */
		if (pos_backWing.y < body_head->pos.y)
		{
			pos_backWing.y += 1.3;
			if (pos_backWing.y > body_head->pos.y) // finish
			{
				pos_backWing.y = body_head->pos.y;
				readyWing = true;
				Effector::PlaySpread(body_leftWing->pos.x, body_leftWing->pos.y,
					GetRand(100), eSpread_SmallBlue);
				Effector::PlaySpread(body_rightWing->pos.x, body_rightWing->pos.y,
					GetRand(100), eSpread_SmallBlue);
			}
		}
	}

	/* tail start */
	if (readyWing)
	{
		if (body_tail00->pos.y - body_head->pos.y < 104)
			body_tail00->pos.y += 1;
		if (body_tail01->pos.y - body_head->pos.y < 167)
			body_tail01->pos.y += 1;

		if (body_tail00->pos.y - body_head->pos.y >= 104 &&
			body_tail01->pos.y - body_head->pos.y >= 167) // finish
		{ 
			state = eState::normal;
			c_start = 0;
		}
	}
}


void BossB::Update_Normal()
{
	/* Time count */
	++c_normal;
	if (c_normal >= NORMAL_ALL_TIME)
	{
		c_normal = 0;
		state = eState::stay;
	}

	/* Move */
	body_head->pos.x = 320 + std::cos(c_move) * 10;

	/* Fire */

}


void BossB::Update_Stay()
{
	/* Time count */
	++c_stay;
	if (c_stay >= STAY_ALL_TIME)
	{
		c_stay = 0;
		state = eState::normal;
	}
}


void BossB::Update_Weak()
{
	/* Time count */
	++c_weak;
	if (c_weak >= WEAK_ALL_TIME + 1)
		c_weak = 0;
}


void BossB::Update_Dead()
{
	/* Time count */
	++c_dead;
	if (c_dead >= DEAD_ALL_TIME)
		isExist = false;
}


BossB::Body::Body(ePart part)
{
	this->part = part;
	c_dead = 0;
	elapsedTime = 0;
	isExist = true;

	switch (part)
	{
	case BossB::ePart::head:
		hitRange = 7;
		hp = 4000;
		break;
	case BossB::ePart::leftWing:
		hitRange = 7;
		hp = 2000;
		break;
	case BossB::ePart::rightWing:
		hitRange = 7;
		hp = 2000;
		break;
	case BossB::ePart::tail00:
		hitRange = 6;
		hp = 1000;
		break;
	case BossB::ePart::tail01:
		hitRange = 6;
		hp = 1000;
		break;
	case BossB::ePart::gun:
		hitRange = 3;
		hp = 20;
		break;
	}
}


void BossB::Body::Update()
{
	++elapsedTime;

	if (hp <= 0)
	{
		++c_dead;
		if (c_dead == 120)
		{
			isExist = false;
			Effector::PlayAnime(pos.x, pos.y, eExplosion_small);
			Effector::PlayAnime(pos.x, pos.y, eExplosion_small);
		}
	}

	switch (part)
	{
	case BossB::ePart::head:
		break;
	case BossB::ePart::leftWing:
		break;
	case BossB::ePart::rightWing:
		break;
	case BossB::ePart::tail00:
		break;
	case BossB::ePart::tail01:
		break;
	case BossB::ePart::gun:
		break;
	}
}


void BossB::Body::Draw(const BossB& boss)
{
	if (hp <= 0)
	{
		if (elapsedTime % 4 >= 2)
			SetDrawBright(255, 0, 0);
	}

	switch (part)
	{
	case BossB::ePart::head:
		boss.img->DrawRota(pos.x, pos.y, 2, 0, "head");
		break;
	case BossB::ePart::leftWing:
		boss.img->DrawRota(pos.x, pos.y, 2, 0, "Lwing");
		break;
	case BossB::ePart::rightWing:
		boss.img->DrawRota(pos.x, pos.y, 2, 0, "Lwing", true, true);
		break;
	case BossB::ePart::tail00:
		boss.img->DrawRota(pos.x, pos.y, 2, 0, "tail00");
		break;
	case BossB::ePart::tail01:
		boss.img->DrawRota(pos.x, pos.y, 2, 0, "tail01");
		break;
	case BossB::ePart::gun:
		boss.img->DrawRota(pos.x, pos.y, 2, 0, "gun");
		break;
	}

	SetDrawBright(255, 255, 255);
}

void BossB::Body::Damage(int point)
{
	hp -= point;

	if (hp <= 0)
		Effector::PlayAnime(pos.x, pos.y, eExplosion_small);
}

bool BossB::Body::HitCheck(const double & ColX, const double & ColY, const int & DamagePoint)
{
	if (hp <= 0)
		return false;
	const bool& IS_HIT = (Vector2D::CirclePointCollision(pos.x, pos.y, ColX, ColY, hitRange));

	if (IS_HIT)
	{
		isDamage = true;
		Damage(DamagePoint);
		IScore::AddScore(1000);
	}

	return IS_HIT;
}


void BossB::Body::Initialize()
{
}
