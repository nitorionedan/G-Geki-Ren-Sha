#include "ZakoEnemy.hpp"
#include "Graphics2D.hpp"
#include "Stage.hpp"
#include "IScore.hpp"
#include "Keyboard.hpp"
#include "Player.hpp"
#include "EneShot.hpp"
#include "Effector.hpp"
#include "ExplosionEffect.hpp"
#include "PieceEffect.hpp"
#include "ItemMng.hpp"
#include "Bezier.hpp"
#include "FileDef.h"
#include <DxLib.h>
#include <cassert>
#include <cmath>

namespace
{
	constexpr int SCREEN_LIMIT_XL = -30;
	constexpr int SCREEN_LIMIT_XR = 640 + 30;
	constexpr int SCREEN_LIMIT_YT = SCREEN_LIMIT_XL;
	constexpr int SCREEN_LIMIT_YB = 480 + 120;

	auto RadToAng = [](double rad) {
		return rad * 180 / DX_PI;
	};

	auto AngToRad = [](double ang) {
		return ang * DX_PI / 180;
	};
}

using namespace MyFile;

// ==========================================================Ze
int ZakoEnemy_Ze::num = 0;

ZakoEnemy_Ze::ZakoEnemy_Ze(const tEnemyData & data)
	: Speed(4)
	, Brake(0.03)
	, Easing(0.99999999)
{
	int gh_tmp = LoadGraph(Gr::ENEMY_ZE);
	switch (data.m_pattern) 
	{
	case 0:	gh = DerivationGraph(0,   0,  29, 19, gh_tmp); break;
	case 1: gh = DerivationGraph(30,  0,  29, 19, gh_tmp); break;
	case 2: gh = DerivationGraph(60,  0,  29, 19, gh_tmp); break;
	case 4: gh = DerivationGraph(0,   20, 29, 19, gh_tmp); break;
	case 5: gh = DerivationGraph(30,  20, 29, 19, gh_tmp); break;
	case 6: gh = DerivationGraph(60,  20, 29, 19, gh_tmp); break;
	default: assert(!"out of range (ZakoEnemy_Ze)");
	}
	DeleteGraph(gh_tmp);

	this->data.param = data;
	this->data.rad = 0;
	this->data.pos.SetVec(data.x_pos, data.y_pos);
	this->data.vspeed.SetVec(2., 4.);
	this->data.vangle = 0;
	this->data.hitRange = 20;
	this->data.isExist = false;
	this->data.isUngry = false;
	this->data.isHit = false;
	this->data.isMove = true;
	this->data.time = 0;
	this->data.loopTime = 0;

	++num;
	id = num;
	maxHP = data.hp;
}

ZakoEnemy_Ze::~ZakoEnemy_Ze()
{
	DeleteGraph(gh);
}

void ZakoEnemy_Ze::Update()
{
	/* Move */
	switch (data.param.m_pattern)
	{
	case 0: Move0(); break;
	default: assert(!"out of range in Ze");
	}

	Fire();
}

void ZakoEnemy_Ze::Draw()
{
	switch (data.param.m_pattern)
	{
	case 0: 
	default: break;
	}
	DrawRotaGraph(data.pos.x, data.pos.y, 2., data.rad, gh, true);
}

void ZakoEnemy_Ze::StartCheck()
{
	// 登場時間が来たら出てくる
	if (IStage::GetTime() == data.param.in_time)
		data.isExist = true;
}

void ZakoEnemy_Ze::Move0()
{
	const bool& IS_IN  = (data.time >= 0 && data.time <= data.param.stop_time);
	const bool& IS_OUT = (data.time >= data.param.out_time);

	// 最初の移動
	if (IS_IN)
	{
		// プレイヤーの前か画面の上半分なら
		if (IPlayer::GetPos().y - 60. > data.pos.y || data.pos.y >= 320.)
			data.vspeed.y = (IPlayer::GetPos().y - 60. - data.pos.y) * Brake;	// 減速

		// 降りる
		if (data.isMove)
			data.pos.y += data.vspeed.y;

		// 横移動：左から右
		if (data.pos.x < IPlayer::GetPos().x - 10. && data.isMove)
		{
			data.vspeed.x = (IPlayer::GetPos().x - 20. - data.pos.x) * Brake;
			data.pos.x += data.vspeed.x;
		}

		// 横移動２：右から左
		if (data.pos.x > IPlayer::GetPos().x + 10. && data.isMove)
		{
			data.pos.x -= data.vspeed.x;
			data.vspeed.x *= Easing;
		}

		// 速度を負にしない
		if (data.vspeed.x < 0.)
		{
			data.vspeed.x = 0.;
			data.isMove = false;
		}
		if (data.vspeed.y < 0.)	data.vspeed.y = 0.;
	}

	// 自機に向く
	if (data.time < data.param.out_time)
		data.rad = atan2(IPlayer::GetPos().y - data.pos.y, IPlayer::GetPos().x - data.pos.x) - DX_PI / 2;	// 自機に向く

	// スピード変更
	if (data.time == data.param.out_time)
		data.vspeed.y = 0.;

	// go home lol
	if (data.time >= data.param.out_time)
	{
		// 加速
		if (data.vspeed.y > -Speed)
			data.vspeed.y -= 0.05;

		// 移動
		data.pos.y += data.vspeed.y;

		// 方向転換のつもり（納得いかない動きなので修正する計画）
		if (data.rad >= 0.)	data.rad += 0.02;
		if (data.rad < 0.)	data.rad -= 0.02;
	}
	
	// go home
	if (IS_OUT)
	{
		if (data.pos.y > -40)
			return;
		data.isExist = false;
		IEnemyMng::CountDownEneNum();
	}
}

void ZakoEnemy_Ze::Fire()
{
	const double& ANGLE = atan2(IPlayer::GetPos().y - data.pos.y, IPlayer::GetPos().x - data.pos.x);

	if (data.time == 20)
	{
		int dir = GetRand(1);
		double addAng = (GetRand(3) / 15.);
		if (dir == 0)
			IEneShot::Fire_Ang(eShotType::normal, data.pos, 0, data.param.s_speed, ANGLE - addAng, 1, 0);
		else
			IEneShot::Fire_Ang(eShotType::normal, data.pos, 0, data.param.s_speed, ANGLE + addAng, 1, 0);
	}

	if (data.time == 30)
	{
		int dir = GetRand(1);
		double addAng = (GetRand(3) / 15.);
		if (dir == 0)
			IEneShot::Fire_Ang(eShotType::normal, data.pos, 0, data.param.s_speed, ANGLE - addAng, 1, 0);
		else
			IEneShot::Fire_Ang(eShotType::normal, data.pos, 0, data.param.s_speed, ANGLE + addAng, 1, 0);
	}

	// 3 ways shot
	if (data.time == data.param.out_time && data.isUngry)
	{
		int dir = GetRand(1);
		double addAng = (GetRand(3) / 15);
		if (dir == 0)
		{
			IEneShot::Fire_Ang(eShotType::normal, data.pos, 0, data.param.s_speed, (ANGLE + 0.3) - addAng, 1, 0);
			IEneShot::Fire_Ang(eShotType::normal, data.pos, 0, data.param.s_speed, ANGLE - addAng, 1, 0);
			IEneShot::Fire_Ang(eShotType::normal, data.pos, 0, data.param.s_speed, (ANGLE - 0.3) - addAng, 1, 0);
		}
		else
		{
			IEneShot::Fire_Ang(eShotType::normal, data.pos, 0, data.param.s_speed, (ANGLE + 0.3) - addAng, 1, 0);
			IEneShot::Fire_Ang(eShotType::normal, data.pos, 0, data.param.s_speed, ANGLE - addAng, 1, 0);
			IEneShot::Fire_Ang(eShotType::normal, data.pos, 0, data.param.s_speed, (ANGLE - 0.3) - addAng, 1, 0);
		}
	}
}

bool ZakoEnemy_Ze::HitCheck(const double & Range, const Vector2D pos, const double & damage)
{
	const bool& IsHit = Vector2D::CirclesCollision(this->data.hitRange, Range, this->data.pos, pos);
	if (IsHit)
		CalcDamage(damage);
	return IsHit;
}

bool ZakoEnemy_Ze::HitCheck(const double & Range, const double X, const double Y, const double Damage)
{
	if (!data.isExist)	return false;

	const bool& IS_HIT = Vector2D::CirclesCollision(Range, data.hitRange, X, Y, data.pos.x, data.pos.y);
	if (IS_HIT)	this->CalcDamage(Damage);
	return IS_HIT;
}

bool ZakoEnemy_Ze::HitCheck(const double X, const double Y, const double Damage)
{
	if (!data.isExist)	return false;
	bool isHit = Vector2D::CirclesCollision(data.hitRange, 6, data.pos.x, data.pos.y + 9., X, Y);
	return isHit;
}

void ZakoEnemy_Ze::CalcDamage(int damage)
{
	data.param.hp -= damage;
	data.isHit = true;
	IScore::AddScore(1);

	// 元気ならここで返す
	if (data.param.hp > 0)	return;

	//ItemDrop();
	data.isExist = false;
	IScore::AddScore(400);
	Effector::PlayAnime(data.pos.x, data.pos.y, eExplosion_small);
	IEnemyMng::CountDownEneNum();
}


// ==========================================================Career
ZakoEnemy_Career::ZakoEnemy_Career(const tEnemyData & data)
{
	LoadDivGraph(Gr::ENEMY_CAREER, _countof(gh), 4, 2, 124, 78, gh);
	gh_fire = LoadGraph(MyFile::Gr::FIRE);
	this->data.param = data;
	this->data.rad = 0;
	this->data.pos.SetVec(data.x_pos, data.y_pos);
	this->data.hitRange = 50;
	this->data.vspeed.SetVec(2., 4.);
	this->data.vangle = 0;
	this->data.isExist = false;
	this->data.isUngry = false;
	this->data.isHit = false;
	this->data.isMove = true;
	this->data.time = 0;
	this->data.loopTime = 0;
	maxHP = data.hp;
}

ZakoEnemy_Career::~ZakoEnemy_Career()
{
	for (auto i : gh)
		DeleteGraph(i);
	DeleteGraph(gh_fire);
}

void ZakoEnemy_Career::Update()
{
	Fire();
	Move();
}

void ZakoEnemy_Career::Draw()
{
	DrawAnime(data.pos.x, data.pos.y, 2., 0., data.time, _countof(gh), 8, gh);

	for (int i = 0; i < 3; ++i)
	{
		if (data.loopTime != i * 20)
			continue;
		DrawRotaGraph(data.pos.x + 70, data.pos.y - 30, 2, 0, gh_fire, TRUE);
		DrawRotaGraph(data.pos.x + 50, data.pos.y,      2, 0, gh_fire, TRUE);
		DrawRotaGraph(data.pos.x - 70, data.pos.y - 30, 2, 0, gh_fire, TRUE);
		DrawRotaGraph(data.pos.x - 50, data.pos.y,      2, 0, gh_fire, TRUE);
	}
}

void ZakoEnemy_Career::StartCheck()
{
	// 登場時間が来たら出てくる
	if (IStage::GetTime() == data.param.in_time)
		data.isExist = true;
}

bool ZakoEnemy_Career::HitCheck(const double & Range, const Vector2D pos, const double & damage)
{
	const bool& IsHit = Vector2D::CirclesCollision(this->data.hitRange, Range, this->data.pos, pos);
	if (IsHit)
		CalcDamage(damage);
	return IsHit;
}

bool ZakoEnemy_Career::HitCheck(const double & Range, const double X, const double Y, const double Damage)
{
	if (!data.isExist)	return false;

	const bool& IS_HIT = Vector2D::CirclesCollision(Range, data.hitRange, X, Y, data.pos.x, data.pos.y);

	if (IS_HIT)	this->CalcDamage(Damage);

	return IS_HIT;
}

bool ZakoEnemy_Career::HitCheck(const double X, const double Y, const double Damage)
{
	if (!data.isExist)	return false;
	bool isHit = Vector2D::CirclesCollision(data.hitRange, 6, data.pos.x, data.pos.y + 9., X, Y);
	return isHit;
}

void ZakoEnemy_Career::CalcDamage(int damage)
{
	data.param.hp -= damage;
	data.isHit= true;
	IScore::AddScore(1);

	// 元気ならここで返す
	if (data.param.hp > 0)	return;

	data.isExist = false;

	IScore::AddScore(1500);
	Effector::PlayAnime( data.pos.x, data.pos.y,               eExplosion_normal);
	Effector::PlaySpread(data.pos.x, data.pos.y, GetRand(100), eSpread_Bigs);
	Effector::PlaySpread(data.pos.x, data.pos.y, GetRand(100), eSpread_SmallBlue);
	IEnemyMng::CountDownEneNum();

	if (data.pos.y < 0)
	{
		IItemMng::Create(Vector2D(data.pos.x, data.pos.y + 60), eItem::Bomber);
		IItemMng::Create(Vector2D(data.pos.x, data.pos.y + 60), eItem::Power);
		IItemMng::Create(Vector2D(data.pos.x, data.pos.y + 60), eItem::Shield);
	}
	else 
	{
		IItemMng::Create(Vector2D(data.pos.x, data.pos.y + 20), eItem::Bomber);
		IItemMng::Create(Vector2D(data.pos.x, data.pos.y + 20), eItem::Power);
		IItemMng::Create(Vector2D(data.pos.x, data.pos.y + 20), eItem::Shield);
	}
} // func end

void ZakoEnemy_Career::Fire()
{
	/* snipe shot */
	if (data.loopTime == 0)
		data.vangle = atan2(IPlayer::GetPos().y - data.pos.y, IPlayer::GetPos().x - data.pos.x);

	/* 4WAY x 2 */
	for (int i = 0; i < 3; ++i)
	{
		if (data.loopTime != i * 20)
			continue;

		/* right side */
		IEneShot::Fire_Ang(eShotType::star, Vector2D::GetVec(data.pos.x + 70, data.pos.y - 30), -0.2, data.param.s_speed, DX_PI / 8., 1, 0);
		IEneShot::Fire_Ang(eShotType::star, Vector2D::GetVec(data.pos.x + 70, data.pos.y - 30), -0.2, data.param.s_speed, DX_PI / 6., 1, 0);
		IEneShot::Fire_Ang(eShotType::star, Vector2D::GetVec(data.pos.x + 50, data.pos.y), -0.2,      data.param.s_speed, DX_PI / 4., 1, 0);
		IEneShot::Fire_Ang(eShotType::star, Vector2D::GetVec(data.pos.x + 50, data.pos.y), -0.2,      data.param.s_speed, DX_PI / 3., 1, 0);

		/* left side */
		IEneShot::Fire_Ang(eShotType::star, Vector2D::GetVec(data.pos.x - 70, data.pos.y - 30), -0.2, data.param.s_speed, DX_PI * 0.85, 1, 0);
		IEneShot::Fire_Ang(eShotType::star, Vector2D::GetVec(data.pos.x - 70, data.pos.y - 30), -0.2, data.param.s_speed, DX_PI * 0.8, 1, 0);
		IEneShot::Fire_Ang(eShotType::star, Vector2D::GetVec(data.pos.x - 50, data.pos.y), -0.2,      data.param.s_speed, DX_PI * 0.7, 1, 0);
		IEneShot::Fire_Ang(eShotType::star, Vector2D::GetVec(data.pos.x - 50, data.pos.y), -0.2,      data.param.s_speed, DX_PI * 0.6, 1, 0);
	}

	/* snipe shot */
	for (int i = 0; i < 4; ++i)
	{
		if (data.loopTime == 20 + (i * 10))
			IEneShot::Fire_Ang(eShotType::wave, data.pos, 0, data.param.s_speed + 2, data.vangle, 1, 0);
	}
}

void ZakoEnemy_Career::Move()
{
	static float c_move = 0.f;
	c_move += 0.01f;

	data.vspeed.SetVec(0.7, 0.2);

	if (data.pos.y < 240) // ぐねぐね
	{
		if (data.loopTime < data.param.stop_time ||
			data.loopTime > data.param.stop_time + 40)
		{
			data.pos.y += data.vspeed.y;
		}
		data.pos.x += data.vspeed.x * std::cos(c_move);
	}
	else // 素早く降りる
		data.pos.y += data.vspeed.y * 6;

	if (data.pos.y > SCREEN_LIMIT_YB)
	{
		data.isExist = false;
		IEnemyMng::CountDownEneNum();
	}
}


// ==========================================================Raide
ZakoEnemy_Den::ZakoEnemy_Den(const tEnemyData & data)
{
	LoadDivGraph(Gr::ENEMY_DE00, _countof(gh), 4, 1, 31, 16, gh);
	this->data.param = data;
	this->data.rad = 0;
	this->data.pos.SetVec(data.x_pos, data.y_pos);
	this->data.vspeed.SetVec(2., 0.1);
	this->data.vangle = 0;
	this->data.hitRange = 20;
	this->data.isExist = false;
	this->data.isUngry = false;
	this->data.isHit = false;
	this->data.isMove = true;
	this->data.time = 0;
	this->data.loopTime = 0;

	maxHP = data.hp;
}

ZakoEnemy_Den::~ZakoEnemy_Den()
{
	for (auto i : gh)
		DeleteGraph(i);
}

void ZakoEnemy_Den::Update()
{
	data.vspeed.SetVec(0, 2);
	data.pos += data.vspeed;

	if (data.pos.y > 490)
	{
		data.isExist = false;
		IEnemyMng::CountDownEneNum();
	}
}

void ZakoEnemy_Den::Draw()
{
	DrawAnime(data.pos.x, data.pos.y, 2, data.rad, data.time, _countof(gh), 2, gh);
}

void ZakoEnemy_Den::StartCheck()
{
	// 登場時間が来たら出てくる
	if (IStage::GetTime() == data.param.in_time)
		data.isExist = true;
}

bool ZakoEnemy_Den::HitCheck(const double & Range, const Vector2D pos, const double & damage)
{
	const bool& IsHit = Vector2D::CirclesCollision(this->data.hitRange, Range, this->data.pos, pos);
	if (IsHit)
		CalcDamage(damage);
	return IsHit;
}

bool ZakoEnemy_Den::HitCheck(const double & Range, const double X, const double Y, const double Damage)
{
	if (!data.isExist)	return false;
	bool isHit = Vector2D::CirclesCollision(data.hitRange, 6, data.pos.x, data.pos.y + 9., X, Y);
	return isHit;
}

bool ZakoEnemy_Den::HitCheck(const double X, const double Y, const double Damage)
{
	return false;
}

void ZakoEnemy_Den::CalcDamage(int damage)
{
	data.param.hp -= damage;

	if (data.param.hp <= 0)
	{
		Effector::PlayAnime(data.pos.x, data.pos.y, eExplosion_small);
		data.isExist = false;
	}
}

void ZakoEnemy_Den::Move()
{
}

void ZakoEnemy_Den::Fire()
{
}


// ==========================================================Flower
ZakoEnemy_Flower::ZakoEnemy_Flower(const tEnemyData & data)
{
}

ZakoEnemy_Flower::~ZakoEnemy_Flower()
{
}

void ZakoEnemy_Flower::Update()
{
}

void ZakoEnemy_Flower::Draw()
{
}

void ZakoEnemy_Flower::StartCheck()
{
}

bool ZakoEnemy_Flower::HitCheck(const double & Range, const Vector2D pos, const double & damage)
{
	return false;
}

bool ZakoEnemy_Flower::HitCheck(const double & Range, const double X, const double Y, const double Damage)
{
	return false;
}

bool ZakoEnemy_Flower::HitCheck(const double X, const double Y, const double Damage)
{
	return false;
}

void ZakoEnemy_Flower::CalcDamage(int damage)
{
}

void ZakoEnemy_Flower::Move()
{
}

void ZakoEnemy_Flower::Fire()
{
}


// ==========================================================Flower
