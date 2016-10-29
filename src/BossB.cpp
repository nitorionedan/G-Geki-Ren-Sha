#include "BossB.hpp"
#include "FileDef.h"
#include "EneShot.hpp"
#include "Effector.hpp"
#include "PieceEffect.hpp"
#include "ExplosionEffect.hpp"
#include "IScore.hpp"
#include "DebugMode.hpp"
#include "Stage.hpp"
#include <DxLib.h>
#include <cmath>
#include <cassert>

namespace
{
	constexpr int MAX_HP = 4000;
	constexpr int START_ALL_TIME  = 3000;
	constexpr int NORMAL_ALL_TIME = 1800;
	constexpr int STAY_ALL_TIME   = 240;
	constexpr int WEAK_ALL_TIME = 300;
	constexpr int DEAD_ALL_TIME = 530;

	constexpr double START_SPEED = 0.7;
	constexpr double ADJUST_RANGE_POS = 90;
	constexpr double TAIL00_POS_Y = 104;
	constexpr double TAIL01_POS_Y = 187;

	constexpr double GUN00_X = 60;
	constexpr double GUN00_Y = 40;

	constexpr double GUN01_X = 180;
	constexpr double GUN01_Y = 30;

	constexpr double GUN02_X = -GUN00_X;
	constexpr double GUN02_Y = GUN00_Y;
	constexpr double GUN03_X = -GUN01_X;
	constexpr double GUN03_Y = GUN01_Y;
}


BossB::BossB()
	: body_head(new Body(ePart::head))
	, body_leftWing(new Body(ePart::leftWing))
	, body_rightWing(new Body(ePart::rightWing))
	, body_tail00(new Body(ePart::tail00))
	, body_tail01(new Body(ePart::tail01))
	, body_gun00(new Body(ePart::gun))
	, body_gun01(new Body(ePart::gun))
	, body_gun02(new Body(ePart::gun))
	, body_gun03(new Body(ePart::gun))
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
	sh_dead = LoadSoundMem(MyFile::Se::LARGE_EX);

	body_head->pos.SetVec(320, -200);
	body_leftWing->pos = body_head->pos;
	body_rightWing->pos = body_head->pos;
	body_tail00->pos = Vector2D::GetVec(body_head->pos.x + 3, body_head->pos.y);
	body_tail01->pos = Vector2D::GetVec(body_head->pos.x + 3, body_head->pos.y);
	pos_backWing = Vector2D::GetVec(body_head->pos.x, body_head->pos.y - 50);

	/* guns */
	body_gun00->pos = Vector2D::GetVec(body_head->pos.x + 10, body_head->pos.y);
	body_gun01->pos = Vector2D::GetVec(body_head->pos.x + 30, body_head->pos.y);
	body_gun02->pos = Vector2D::GetVec(body_head->pos.x - 10, body_head->pos.y);
	body_gun03->pos = Vector2D::GetVec(body_head->pos.x - 30, body_head->pos.y);

	hp = MAX_HP;
	elapsedTime = 0;
	c_start = 0;
	c_normal = 0;
	c_stay = 0;
	c_weak = 0;
	c_dead = 0;
	c_move_y = 0;
	alpha_gun = 0;
	readyWing = false;
	isExist = true;
	isDead = false;

	state = eState::start;
}


BossB::~BossB()
{
	DeleteSoundMem(sh_dead);
}


void BossB::Update()
{
	++elapsedTime;

	if (isExist == false)
		return;

	switch (state)
	{
	case BossB::eState::start: Update_Start();  break;
	case BossB::eState::normal:Update_Normal();	break;
	case BossB::eState::stay: Update_Stay();    break;
	case BossB::eState::weak: Update_Weak();    break;
	case BossB::eState::dead: Update_Dead();    break;
	}

	/* fire */
	switch (state)
	{
	case BossB::eState::normal:Fire_Normal(); break;
	case BossB::eState::stay: Fire_Stay();    break;
	case BossB::eState::weak: Fire_Weak();    break;
	}

	/* weak */
	if (body_leftWing->isExist == false && 
		body_rightWing->isExist == false &&
		body_tail00->isExist == false &&
		body_tail01->isExist == false &&
		state != eState::weak &&
		body_head->hp > 0)
	{
		state = eState::weak;
	}

	/* wing death */
	if (body_leftWing->isExist == false)
	{
		if (body_gun02->isExist)
			body_gun02->Damage(10000);
		if (body_gun03->isExist)
			body_gun03->Damage(10000);
	}
	if (body_rightWing->isExist == false)
	{
		if (body_gun00->isExist)
			body_gun00->Damage(10000);
		if (body_gun01->isExist)
			body_gun01->Damage(10000);
	}

	/* dead */
	if (body_head->hp <= 0 && state != eState::dead)
	{
		state = eState::dead;
		isDead = true;
		body_leftWing-> Damage(10000);
		body_rightWing->Damage(10000);
		body_tail00->   Damage(10000);
		body_tail01->   Damage(10000);
		body_gun00->    Damage(10000);
		body_gun00->    Damage(10000);
		body_gun01->    Damage(10000);
		body_gun02->    Damage(10000);
		body_gun03->    Damage(10000);
	}

	clsDx();
	if (body_leftWing->isExist == false)
		printfDx("l_Wing exist\n");
	if (body_rightWing->isExist == false)
		printfDx("r_Wing exist\n");
	if (body_tail00->isExist == false )
		printfDx("tail00 exist\n");
	if (body_tail01->isExist == false)
		printfDx("tail01 exist\n");
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
			body_leftWing->Draw(*this);
			body_rightWing->Draw(*this);
			body_tail01->Draw(*this);
			body_tail00->Draw(*this);
			body_head->Draw(*this);
			body_gun00->Draw(*this);
			body_gun01->Draw(*this);
			body_gun02->Draw(*this);
			body_gun03->Draw(*this);
		}
		else
		{
			img->DrawRota(pos_backWing.x, pos_backWing.y, 2, 0, "wing_back");
			img->DrawRota(body_head->pos.x, body_head->pos.y, 2, 0, "back", TRUE);
		}
		break;
	case BossB::eState::normal:
		body_leftWing->Draw(*this);
		body_rightWing->Draw(*this);
		body_tail01->Draw(*this);
		body_tail00->Draw(*this);
		body_head->Draw(*this);
		body_gun00->Draw(*this);
		body_gun01->Draw(*this);
		body_gun02->Draw(*this);
		body_gun03->Draw(*this);
		break;
	case BossB::eState::stay:
		body_leftWing->Draw(*this);
		body_rightWing->Draw(*this);
		body_tail01->Draw(*this);
		body_tail00->Draw(*this);
		body_head->Draw(*this);
		body_gun00->Draw(*this);
		body_gun01->Draw(*this);
		body_gun02->Draw(*this);
		body_gun03->Draw(*this);
		break;
	case BossB::eState::weak:
		body_leftWing->Draw(*this);
		body_rightWing->Draw(*this);
		body_tail01->Draw(*this);
		body_tail00->Draw(*this);
		body_head->Draw(*this);
		body_gun00->Draw(*this);
		body_gun01->Draw(*this);
		body_gun02->Draw(*this);
		body_gun03->Draw(*this);
		break;
	case BossB::eState::dead:
		body_leftWing->Draw(*this);
		body_rightWing->Draw(*this);
		body_tail01->Draw(*this);
		body_tail00->Draw(*this);
		body_head->Draw(*this);
		body_gun00->Draw(*this);
		body_gun01->Draw(*this);
		body_gun02->Draw(*this);
		body_gun03->Draw(*this);
		break;
	}

	if (DebugMode::isTest)
	{
		DrawCircle(body_head->pos.x, body_head->pos.y, body_head->hitRange, GetColor(0, 0, 255), FALSE);
		DrawFormatString(body_head->pos.x, body_head->pos.y, GetColor(0, 0, 255), "%lf", body_head->hp);
		DrawCircle(body_leftWing->pos.x - ADJUST_RANGE_POS, body_leftWing->pos.y, body_leftWing->hitRange, GetColor(0, 0, 255), FALSE);
		DrawCircle(body_rightWing->pos.x + ADJUST_RANGE_POS, body_rightWing->pos.y, body_rightWing->hitRange, GetColor(0, 0, 255), FALSE);
		DrawCircle(body_tail00->pos.x, body_tail00->pos.y, body_tail00->hitRange, GetColor(0, 0, 255), FALSE);
		DrawCircle(body_tail01->pos.x, body_tail01->pos.y, body_tail01->hitRange, GetColor(0, 0, 255), FALSE);
		DrawCircle(body_gun00->pos.x, body_gun00->pos.y, body_gun00->hitRange, GetColor(0, 0, 255), FALSE);
		DrawFormatString(body_gun00->pos.x, body_gun00->pos.y, GetColor(0, 0, 255), "%lf", body_gun00->hp);
		DrawCircle(body_gun01->pos.x, body_gun01->pos.y, body_gun01->hitRange, GetColor(0, 0, 255), FALSE);
		DrawCircle(body_gun02->pos.x, body_gun02->pos.y, body_gun02->hitRange, GetColor(0, 0, 255), FALSE);
		DrawCircle(body_gun03->pos.x, body_gun03->pos.y, body_gun03->hitRange, GetColor(0, 0, 255), FALSE);
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
	if (body_gun00->HitCheck(ColX, ColY, DamagePoint))
		return true;
	if (body_gun01->HitCheck(ColX, ColY, DamagePoint))
		return true;
	if (body_gun02->HitCheck(ColX, ColY, DamagePoint))
		return true;
	if (body_gun03->HitCheck(ColX, ColY, DamagePoint))
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
		body_head->pos.y      += START_SPEED;
		if (body_head->pos.y >= 90)
			body_head->pos.y = 90;

		body_tail00->pos.y    += START_SPEED;
		body_tail01->pos.y    += START_SPEED;
		body_leftWing->pos.y  += START_SPEED;
		body_rightWing->pos.y += START_SPEED;
		pos_backWing.y        += START_SPEED;
		body_gun00->pos = Vector2D::GetVec(body_head->pos.x + GUN00_X, body_head->pos.y + GUN00_Y);
		body_gun01->pos = Vector2D::GetVec(body_head->pos.x + GUN01_X, body_head->pos.y + GUN01_Y);
		body_gun02->pos = Vector2D::GetVec(body_head->pos.x + GUN02_X, body_head->pos.y + GUN02_Y);
		body_gun03->pos = Vector2D::GetVec(body_head->pos.x + GUN03_X, body_head->pos.y + GUN03_Y);
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

	/* Guns */
	if (readyWing)
	{
		alpha_gun += 2;
		if (alpha_gun > 255)
			alpha_gun = 255;
	}


	/* tail start */
	if (readyWing)
	{
		if (body_tail00->pos.y - body_head->pos.y < TAIL00_POS_Y)
			body_tail00->pos.y += 3;
		if (body_tail01->pos.y - body_head->pos.y < TAIL01_POS_Y)
			body_tail01->pos.y += 5;

		if (body_tail00->pos.y - body_head->pos.y >= TAIL00_POS_Y &&
			body_tail01->pos.y - body_head->pos.y >= TAIL01_POS_Y) // finish
		{ 
			body_tail00->pos.y = body_head->pos.y + TAIL00_POS_Y;
			body_tail01->pos.y = body_head->pos.y + TAIL01_POS_Y;

			Effector::PlaySpread(body_tail00->pos.x, body_tail00->pos.y,
				GetRand(100), eSpread_SmallOrange);
			Effector::PlaySpread(body_tail01->pos.x, body_tail01->pos.y,
				GetRand(100), eSpread_SmallOrange);

			state = eState::normal;
			c_start = 0;
		}
	}

	/* parts update */
	body_head->Update();
	body_leftWing->Update();
	body_rightWing->Update();
	body_tail00->Update();
	body_tail01->Update();
	body_gun00->Update();
	body_gun01->Update();
	body_gun02->Update();
	body_gun03->Update();
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
	body_head->pos.x = 320 + std::sin(c_move) * 140;
	body_head->pos.y = 90 + std::sin(c_move_y) * 30;
	c_move += 0.02;
	c_move_y += 0.04;

	body_leftWing->pos = body_head->pos;
	body_rightWing->pos = body_head->pos;
	body_tail00->pos = Vector2D::GetVec(body_head->pos.x + 3, body_head->pos.y + TAIL00_POS_Y);
	body_tail01->pos = Vector2D::GetVec(body_head->pos.x + 3, body_head->pos.y + TAIL01_POS_Y);
	body_gun00->pos = Vector2D::GetVec(body_head->pos.x + 3, body_head->pos.y);
	pos_backWing = Vector2D::GetVec(body_head->pos.x, body_head->pos.y - 50);
	body_gun00->pos = Vector2D::GetVec(body_head->pos.x + GUN00_X, body_head->pos.y + GUN00_Y);
	body_gun01->pos = Vector2D::GetVec(body_head->pos.x + GUN01_X, body_head->pos.y + GUN01_Y);
	body_gun02->pos = Vector2D::GetVec(body_head->pos.x + GUN02_X, body_head->pos.y + GUN02_Y);
	body_gun03->pos = Vector2D::GetVec(body_head->pos.x + GUN03_X, body_head->pos.y + GUN03_Y);

	/* parts update */
	body_head->Update();
	body_leftWing->Update();
	body_rightWing->Update();
	body_tail00->Update();
	body_tail01->Update();
	body_gun00->Update();
	body_gun01->Update();
	body_gun02->Update();
	body_gun03->Update();
	/* parts update */
	body_tail00->Fire();
	body_tail01->Fire();
	body_gun00->Fire();
	body_gun01->Fire();
	body_gun02->Fire();
	body_gun03->Fire();
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

	/* parts update */
	body_head->Update();
	body_leftWing->Update();
	body_rightWing->Update();
	body_tail00->Update();
	body_tail01->Update();
	body_gun00->Update();
	body_gun01->Update();
	body_gun02->Update();
	body_gun03->Update();
	/* fire */
	body_tail00->Fire();
	body_tail01->Fire();
	body_gun00->Fire();
	body_gun01->Fire();
	body_gun02->Fire();
	body_gun03->Fire();
}


void BossB::Update_Weak()
{
	/* Time count */
	++c_weak;
	if (c_weak >= WEAK_ALL_TIME + 1)
		c_weak = 0;

	/* Move */
	body_head->pos.x = 320 + std::sin(c_move) * 140;
	body_head->pos.y = 90 + std::sin(c_move_y) * 30;
	c_move += 0.02;
	c_move_y += 0.04;

	body_leftWing->pos = body_head->pos;
	body_rightWing->pos = body_head->pos;
	body_tail00->pos = Vector2D::GetVec(body_head->pos.x + 3, body_head->pos.y + TAIL00_POS_Y);
	body_tail01->pos = Vector2D::GetVec(body_head->pos.x + 3, body_head->pos.y + TAIL01_POS_Y);
	body_gun00->pos = Vector2D::GetVec(body_head->pos.x + 3, body_head->pos.y);
	pos_backWing = Vector2D::GetVec(body_head->pos.x, body_head->pos.y - 50);
	body_gun00->pos = Vector2D::GetVec(body_head->pos.x + GUN00_X, body_head->pos.y + GUN00_Y);
	body_gun01->pos = Vector2D::GetVec(body_head->pos.x + GUN01_X, body_head->pos.y + GUN01_Y);
	body_gun02->pos = Vector2D::GetVec(body_head->pos.x + GUN02_X, body_head->pos.y + GUN02_Y);
	body_gun03->pos = Vector2D::GetVec(body_head->pos.x + GUN03_X, body_head->pos.y + GUN03_Y);

	/* parts update */
	body_head->Update();
	body_leftWing->Update();
	body_rightWing->Update();
	body_tail00->Update();
	body_tail01->Update();
	body_gun00->Update();
	body_gun01->Update();
	body_gun02->Update();
	body_gun03->Update();

	/* Fire */
	body_head->Fire();
}


void BossB::Update_Dead()
{
	/* Time count */
	++c_dead;

	if (isExist == false)
		return;

	if (c_dead >= DEAD_ALL_TIME)
	{
		isExist = false;
		Effector::PlaySpecialEplosion(body_head->pos.x, body_head->pos.y);
		IScore::AddScore(50000);
		IStage::Clear();
	}

	const bool& Is_deing = (elapsedTime % 20 == 0);
	if (Is_deing)	// small explosion
		Effector::PlayAnime(body_head->pos.x + GetRand(80) - 40, body_head->pos.y + GetRand(80) - 40, eExplosion_small);

	if (c_dead == 180.)	// final explosion
	{
		for (int i = 0; i < 5; ++i)
			Effector::PlayAnime(body_head->pos.x, body_head->pos.y, eExplosion_big);
		PlaySoundMem(sh_dead, DX_PLAYTYPE_BACK);
	}

	/* parts update */
	body_head->Update();
	body_leftWing->Update();
	body_rightWing->Update();
	body_tail00->Update();
	body_tail01->Update();
	body_gun00->Update();
	body_gun01->Update();
	body_gun02->Update();
	body_gun03->Update();
}


void BossB::Fire_Normal()
{
}


void BossB::Fire_Stay()
{
}


void BossB::Fire_Weak()
{
}


BossB::Body::Body(ePart part)
{
	sh_missile = LoadSoundMem(MyFile::Se::SHOT_MISSILE);

	this->part = part;
	c_dead = 0;
	elapsedTime = 0;
	radAng = 0;
	isExist = true;
	isDamage = false;

	switch (part)
	{
	case BossB::ePart::head:
		hitRange = 28;
		hp = 3000; // 3000
		break;
	case BossB::ePart::leftWing:
		hitRange = 35;
		hp = 2000; // 2000
		break;
	case BossB::ePart::rightWing:
		hitRange = 35;
		hp = 2000; // 2000
		break;
	case BossB::ePart::tail00:
		hitRange = 24;
		hp = 1000; // 1000
		break;
	case BossB::ePart::tail01:
		hitRange = 24;
		hp = 1000; // 1000
		break;
	case BossB::ePart::gun:
		hitRange = 14;
		hp = 300; // 300
		break;
	default: assert(!"no defined");
	}
	max_hp = hp;
}

BossB::Body::~Body()
{
	DeleteSoundMem(sh_missile);
}


void BossB::Body::Update()
{
	++elapsedTime;

	if (isExist == false)
		return;

	if (part == ePart::gun)
		radAng = std::atan2(IPlayer::GetPos().y - pos.y, IPlayer::GetPos().x - pos.x) - 90 * DX_PI / 180;

	if (hp <= 0)
	{
		++c_dead;
		if (c_dead == 120 && part != ePart::head)
		{
			isExist = false;
			if (part == ePart::leftWing || part == ePart::rightWing)
			{
				Effector::PlayAnime(pos.x, pos.y, eExplosion_normal);
				IScore::AddScore(2000);
			}
			else
			{
				Effector::PlayAnime(pos.x, pos.y, eExplosion_small);
				Effector::PlayAnime(pos.x, pos.y, eExplosion_small);
				IScore::AddScore(500);
			}
		}

		if (c_dead == DEAD_ALL_TIME && part == ePart::head)
		{
			isExist = false;
			IScore::AddScore(3000);
		}
	}

	if (HitCheckToPlayer())
		Damage(1);
}


void BossB::Body::Draw(const BossB& boss)
{
	if (isExist == false)
		return;

	/* weak flush */
	if (hp <= max_hp / 3.)
	{
		if (elapsedTime % 12 >= 8)
		{
			SetDrawBright(255, 0, 0);
			if (part == ePart::leftWing || part == ePart::rightWing)
				SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255);
		}
	}

	if (isDamage)
		SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255);

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
		boss.img->DrawRota(pos.x, pos.y, 2, radAng, "gun");
		break;
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	SetDrawBright(255, 255, 255);
	isDamage = false;
}

void BossB::Body::Damage(int point)
{
	hp -= point;
	isDamage = true;

	if (hp <= 0)
		Effector::PlayAnime(pos.x, pos.y, eExplosion_small);
}

bool BossB::Body::HitCheckToPlayer() {
	bool isHit;
	if (part == ePart::leftWing)
		isHit = IPlayer::HitCheckCircl(hitRange - ADJUST_RANGE_POS, pos);
	else if (part == ePart::rightWing)
		isHit = IPlayer::HitCheckCircl(hitRange + ADJUST_RANGE_POS, pos);
	else
		isHit = IPlayer::HitCheckCircl(hitRange, pos);
	return isHit;
}

bool BossB::Body::HitCheck(const double & ColX, const double & ColY, const int & DamagePoint)
{
	if (hp <= 0)
		return false;
	bool IS_HIT = false;
	
	if (part == ePart::leftWing)
		IS_HIT = (Vector2D::CirclePointCollision(pos.x - ADJUST_RANGE_POS, pos.y, ColX, ColY, hitRange));
	else if (part == ePart::rightWing)
		IS_HIT = (Vector2D::CirclePointCollision(pos.x + ADJUST_RANGE_POS, pos.y, ColX, ColY, hitRange));
	else
		IS_HIT = (Vector2D::CirclePointCollision(pos.x, pos.y, ColX, ColY, hitRange));

	if (IS_HIT)
	{
		Damage(DamagePoint);
		IScore::AddScore(1000);
	}

	return IS_HIT;
}

void BossB::Body::Fire()
{
	if (isExist == false)
		return;

	switch (part)
	{
	case BossB::ePart::head:
		Fire_Head();
		break;
	case BossB::ePart::tail00:
		Fire_Tail00();
		break;
	case BossB::ePart::tail01:
		Fire_Tail00();
		break;
	case BossB::ePart::gun:
		Fire_Gun();
		break;
	}
}

void BossB::Body::Fire_Head()
{
	if (elapsedTime % 8 == 0)
	{
		Vector2D tarPos = Vector2D::GetVec(GetRand(640), GetRand(480));
		Vector2D dir = Vector2D::GetVec2(pos, tarPos);
		Vector2D force = dir.Normalize() * (GetRand(4) + 5);

		IEneShot::Fire(eShotType::normal, pos, 0, force, 1, 0);
	}
}

void BossB::Body::Fire_Gun()
{
	if (elapsedTime % 120 == 0 ||
		elapsedTime % 120 == 6 ||
		elapsedTime % 120 == 12)
	{
		Vector2D tarPos(IPlayer::GetPos().x + GetRand(10) - 20., IPlayer::GetPos().y);
		Vector2D dir = Vector2D::GetVec2(pos, tarPos);
		Vector2D force = dir.Normalize() * 6;
		IEneShot::Fire(eShotType::short_laser, pos, 0, force, 1, 0);
	}
}

void BossB::Body::Fire_Tail00()
{
	if (elapsedTime % 300 == 0)
	{
		Vector2D tarPos = Vector2D::GetVec(GetRand(640), GetRand(480));
		Vector2D dir = Vector2D::GetVec2(pos, tarPos);
		Vector2D force = dir.Normalize() * 4;
		IEneShot::Fire(eShotType::missile, pos, 0, force, 1.01, 2, eShotAI::homing);
		PlaySoundMem(sh_missile, DX_PLAYTYPE_BACK);
	}
}

void BossB::Body::Fire_Tail01()
{
	if (elapsedTime % 300 == 0)
	{
		Vector2D tarPos = Vector2D::GetVec(GetRand(640), GetRand(480));
		Vector2D dir = Vector2D::GetVec2(pos, tarPos);
		Vector2D force = dir.Normalize() * 4;
		IEneShot::Fire(eShotType::missile, pos, 0, force, 1.01, 2, eShotAI::homing);
		PlaySoundMem(sh_missile, DX_PLAYTYPE_BACK);
	}
}


void BossB::Body::Initialize()
{
}
