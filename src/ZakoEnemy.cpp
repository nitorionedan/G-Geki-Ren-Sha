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
#include <algorithm>
#undef max
#undef min

namespace
{
	constexpr int SCREEN_LIMIT_XL = -100;
	constexpr int SCREEN_LIMIT_XR = 640 + 100;
	constexpr int SCREEN_LIMIT_YT = SCREEN_LIMIT_XL;
	constexpr int SCREEN_LIMIT_YB = 480 + 100;

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
	IEnemyMng::OffPacific();
}


// ==========================================================Career==========================================================
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
	c_move = 0;
	hasMissile = true;
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
	IScore::AddScore(1);

	// 元気ならここで返す
	if (data.param.hp > 0)	return;

	data.isExist = false;

	IScore::AddScore(1500);
	Effector::PlayAnime( data.pos.x, data.pos.y,               eExplosion_normal);
	Effector::PlaySpread(data.pos.x, data.pos.y, GetRand(100), eSpread_Bigs);
	Effector::PlaySpread(data.pos.x, data.pos.y, GetRand(100), eSpread_SmallBlue);
	IEnemyMng::CountDownEneNum();
	IEnemyMng::OffPacific();

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

	if (data.pos.y >= 240 && hasMissile)
	{
		hasMissile = false;
		Vector2D dir(-2, 2);
		IEneShot::Fire(eShotType::missile, data.pos, 0, dir, 1.01, 2, eShotAI::homing);
		dir.SetVec(2, 2);
		IEneShot::Fire(eShotType::missile, data.pos, 0, dir, 1.01, 2, eShotAI::homing);
	}
}

void ZakoEnemy_Career::Move()
{
	c_move += 0.01f;

	data.vspeed.SetVec(0.7, 0.4);

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


// ==========================================================Raide==========================================================
ZakoEnemy_Den::ZakoEnemy_Den(const tEnemyData & data)
	: Move_max(400)
{
	LoadDivGraph(Gr::ENEMY_DE00, _countof(gh), 4, 1, 31, 16, gh);
	sh_shot = LoadSoundMem(Se::SHOT_LASER);

	this->data.param = data;
	this->data.rad = 0;
	this->data.pos.SetVec(data.x_pos, data.y_pos);
	this->data.vspeed.SetVec(0, 9);
	this->data.vangle = 0;
	this->data.hitRange = 15;
	this->data.isExist = false;
	this->data.isUngry = false;
	this->data.isHit = false;
	this->data.isMove = true;
	this->data.time = 0;
	this->data.loopTime = 0;

	distance = Move_max - this->data.pos.y;
	brake = this->data.vspeed.y / data.stop_time;
	maxHP = data.hp;
	returnTime = this->data.param.out_time + 50;
	isReturn = false;
	isFire = false;
}

ZakoEnemy_Den::~ZakoEnemy_Den()
{
	for (auto i : gh)
		DeleteGraph(i);
	DeleteSoundMem(sh_shot);
}

void ZakoEnemy_Den::Update()
{
	isFire = false;

	switch (data.param.m_pattern)
	{
	case 0: Move0(); break;
	case 1: Move1(); break;
	default: assert(!"no define");
	}

	switch (data.param.s_pattern)
	{
	case 0: Fire0(); break;
	default: assert(!"no define");
	}

	if (data.pos.x < SCREEN_LIMIT_XL ||
		data.pos.x > SCREEN_LIMIT_XR ||
		data.pos.y < SCREEN_LIMIT_YT ||
		data.pos.y > SCREEN_LIMIT_YB)
	{
		IEnemyMng::CountDownEneNum();
		data.isExist = false;
	}
}

void ZakoEnemy_Den::Draw()
{
	DrawAnime(data.pos.x, data.pos.y, 2, data.rad, data.time, _countof(gh), 2, gh);

	if (isFire)
	{}
}

void ZakoEnemy_Den::StartCheck()
{
	// 登場時間が来たら出てくる
	if (IStage::GetTime() == data.param.in_time)
		data.isExist = true;
}

bool ZakoEnemy_Den::HitCheck(const double & Range, const double X, const double Y, const double Damage)
{
	if (!data.isExist)	return false;
	bool isHit = Vector2D::CirclesCollision(data.hitRange, Range, data.pos.x, data.pos.y + 9., X, Y);
	if (isHit)
		CalcDamage(Damage);
	return isHit;
}

bool ZakoEnemy_Den::HitCheck(const double X, const double Y, const double Damage)
{
	if (!data.isExist)	return false;
	bool isHit = Vector2D::CirclesCollision(data.hitRange, 6, data.pos.x, data.pos.y + 9., X, Y);
	if (isHit)
		CalcDamage(Damage);
	return isHit;
}

void ZakoEnemy_Den::CalcDamage(int damage)
{
	data.param.hp -= damage;

	if (data.param.hp <= 0)
	{
		Effector::PlayAnime(data.pos.x, data.pos.y, eExplosion_small);
		Effector::PlaySpread(data.pos.x, data.pos.y, GetRand(100), eSpread_SmallGrey);
		IEnemyMng::CountDownEneNum();
		IEnemyMng::OffPacific();
		data.isExist = false;
	}
}

void ZakoEnemy_Den::Move0()
{	
	if (data.time < data.param.out_time)
	{
		data.vspeed.y -= brake;  // Braking
		data.pos += data.vspeed; // Add force

		if (data.vspeed.y < 0)
			data.vspeed.y = 0;
	}
	else
	{
		if (data.time == data.param.out_time)
		{
			data.vspeed.SetVec(2, 3);
			isReturn = true;
		}

		if (isReturn && data.time >= returnTime)
		{
			data.pos += data.vspeed;
			data.vspeed.x *= 1.004;
			data.vspeed.y *= 0.979;
		}
	}
}

void ZakoEnemy_Den::Move1()
{
	if (data.time < data.param.out_time)
	{
		data.vspeed.y -= brake;  // Braking
		data.pos += data.vspeed; // Add force

		if (data.vspeed.y < 0)
			data.vspeed.y = 0;
	}
	else
	{
		if (data.time == data.param.out_time)
		{
			data.vspeed.SetVec(-2, 3);
			isReturn = true;
		}

		if (isReturn && data.time >= returnTime)
		{
			data.pos += data.vspeed;
			data.vspeed.x *= 1.004;
			data.vspeed.y *= 0.979;
		}
	}
}

void ZakoEnemy_Den::Fire0()
{
	if (data.time == data.param.out_time + 10 || data.time == data.param.out_time + 20)
	{
		if (std::abs(IPlayer::GetPos().x - data.pos.x) > 200.)
			return;
		Vector2D dir(0, 1);
		Vector2D pos;
		pos = this->data.pos;
		dir = dir * data.param.s_speed;
		
		data.pos.x = this->data.pos.x + 20;
		IEneShot::Fire(eShotType::laser, data.pos, 0, dir, 1, 0);
		data.pos.x = this->data.pos.x - 20;
		IEneShot::Fire(eShotType::laser, data.pos, 0, dir, 1, 0);
		PlaySoundMem(sh_shot, DX_PLAYTYPE_BACK);
	}
}


// ==========================================================Flower
ZakoEnemy_Flower::ZakoEnemy_Flower(const tEnemyData & data)
	: Max_speed_x(4)
	, Max_speed_y(4)
{
	gh_core    = LoadGraph(Gr::ENEMY_FLWR_CORE);
	gh_petal   = LoadGraph(Gr::ENEMY_FLWR_PETAL);
	gh_missile = LoadGraph(Gr::ESHOT_MISSILE);

	this->data.param = data;
	this->data.rad = 0;
	this->data.pos.SetVec(data.x_pos, data.y_pos);
	this->data.vspeed = Vector2D::ZERO;
	this->data.vangle = 0;
	this->data.hitRange = 20;
	this->data.isExist = false;
	this->data.isUngry = false;
	this->data.isHit = false;
	this->data.isMove = true;
	this->data.time = 0;
	this->data.loopTime = 0;

	switch (data.m_pattern)
	{
	case 0: this->data.vspeed.SetVec(0, 4);
	case 1: this->data.vspeed.SetVec(0, 4);
	case 2: this->data.vspeed.SetVec(0, 4);
	case 3: this->data.vspeed.SetVec(0, 4);
	default: break;
	}

	brake = this->data.vspeed.y / (this->data.param.stop_time - GetRand(50));
	radAng = AngToRad(GetRand(360));
	radAng_petal = AngToRad(120);
	addAng = (GetRand(20) - 10.) / 100.;
	hasMissile = true;
	isReturn = false;
}

ZakoEnemy_Flower::~ZakoEnemy_Flower()
{
	DeleteGraph(gh_core);
	DeleteGraph(gh_petal);
	DeleteGraph(gh_missile);
}

void ZakoEnemy_Flower::Update()
{
	switch (data.param.m_pattern)
	{
	case 0: Move0(); break;
	case 1: Move1(); break;
	case 2: Move2(); break;
	case 3: Move3(); break;
	default: assert(!"no defined");
	}

	switch(data.param.s_pattern)
	{
	case 0: Fire0(); break;
	default: assert(!"no defined");
	}

	if (data.pos.x < SCREEN_LIMIT_XL ||
		data.pos.x > SCREEN_LIMIT_XR ||
		data.pos.y < SCREEN_LIMIT_YT ||
		data.pos.y > SCREEN_LIMIT_YB)
	{
		IEnemyMng::CountDownEneNum();
		data.isExist = false;
	}
}

void ZakoEnemy_Flower::Draw()
{
	/* petals */
	for (int i = 1; i < 4; ++i)
		DrawRotaGraph2F(
			data.pos.x + std::cos(radAng + radAng_petal * i) * 16,
			data.pos.y + std::sin(radAng + radAng_petal * i) * 16,
			19.f, 39.f,
			2, radAng + radAng_petal * i, gh_petal, TRUE);

	/* missiles */
	if (hasMissile)
	{
		for (int i = 0; i < 3; ++i)
			DrawRotaGraph(data.pos.x + std::cos(radAng + 1.75 + (i * AngToRad(120))) * 40,
				data.pos.y + std::sin(radAng + 1.75 + (i * AngToRad(120))) * 40,
				2, radAng + 1.75 + AngToRad(90) + (i * AngToRad(120)), gh_missile, TRUE);
	}

	/* core */
	DrawRotaGraph(data.pos.x, data.pos.y, 2, radAng, gh_core, TRUE);
}

void ZakoEnemy_Flower::StartCheck()
{
	if (IStage::GetTime() == data.param.in_time)
		data.isExist = true;		
}

bool ZakoEnemy_Flower::HitCheck(const double & Range, const double X, const double Y, const double Damage)
{
	if (!data.isExist)	return false;
	bool isHit = Vector2D::CirclesCollision(data.hitRange, Range, data.pos.x, data.pos.y + 9., X, Y);
	if (isHit)
		CalcDamage(Damage);
	return isHit;
}

bool ZakoEnemy_Flower::HitCheck(const double X, const double Y, const double Damage)
{
	if (!data.isExist)	return false;
	bool isHit = Vector2D::CirclesCollision(data.hitRange, 6, data.pos.x, data.pos.y + 9., X, Y);
	if (isHit)
		CalcDamage(Damage);
	return isHit;
}

void ZakoEnemy_Flower::CalcDamage(int damage)
{
	data.param.hp -= damage;

	if (data.param.hp <= 0)
	{
		Effector::PlayAnime(data.pos.x, data.pos.y, eExplosion_small);
		Effector::PlaySpread(data.pos.x, data.pos.y, GetRand(100), eSpread_SmallRed);
		IEnemyMng::CountDownEneNum();
		IEnemyMng::OffPacific();
		data.isExist = false;
	}
}

void ZakoEnemy_Flower::Move0()
{
	if (data.time <= data.param.stop_time && data.vspeed.y > 0.)
	{
		data.vspeed.y -= brake;
		data.vspeed.y = std::max(0., data.vspeed.y);

		data.pos += data.vspeed;
		radAng += addAng;
	}
	
	if (data.time > data.param.stop_time && isReturn == false)
	{
		if (data.time == data.param.stop_time + 1)
		{
			data.vspeed.SetZero();
			addAng = 0;
		}

		addAng += 0.005;
		radAng += addAng;
		if (addAng >= 0.5)
			isReturn = true;
	}

	if (isReturn)
	{
		addAng -= 0.008;
		addAng = std::max(0., addAng);
		radAng += addAng;

		if (data.time >= data.param.out_time)
		{
			if (data.vspeed.x < Max_speed_x)
				data.vspeed.x += 0.03;

			if (data.vspeed.x < Max_speed_y)
				data.vspeed.y -= 0.01;

			data.pos += data.vspeed;
		}
	}
}

void ZakoEnemy_Flower::Move1()
{
	if (data.time <= data.param.stop_time && data.vspeed.y > 0.)
	{
		data.vspeed.y -= brake;
		data.vspeed.y = std::max(0., data.vspeed.y);

		data.pos += data.vspeed;
		radAng += addAng;
	}

	if (data.time > data.param.stop_time && isReturn == false)
	{
		if (data.time == data.param.stop_time + 1)
		{
			data.vspeed.SetZero();
			addAng = 0;
		}

		addAng += 0.005;
		radAng += addAng;
		if (addAng >= 0.5)
			isReturn = true;
	}

	if (isReturn)
	{
		addAng -= 0.004;
		addAng = std::max(0., addAng);
		radAng += addAng;

		if (data.time >= data.param.out_time)
		{
			if (data.vspeed.x < Max_speed_x)
				data.vspeed.x -= 0.03;

			if (data.vspeed.x < Max_speed_y)
				data.vspeed.y -= 0.01;

			data.pos += data.vspeed;
		}
	}
}

void ZakoEnemy_Flower::Move2()
{
	/* braking */
	if (data.time <= data.param.stop_time && data.vspeed.y > 0.)
	{
		data.vspeed.y -= brake;
		data.vspeed.y = std::max(0., data.vspeed.y);

		data.pos += data.vspeed;
		radAng += addAng;
	}

	/* return move */
	if (data.time > data.param.stop_time && isReturn == false)
	{
		if (data.time == data.param.stop_time + 1)
		{
			data.vspeed.SetZero();
			addAng = 0;
		}

		addAng += 0.005;
		radAng += addAng;
		if (addAng >= 0.5)
			isReturn = true;
	}

	if (isReturn)
	{
		addAng -= 0.004;
		addAng = std::max(0., addAng);
		radAng += addAng;

		if (data.time >= data.param.out_time)
		{
			if (data.vspeed.x < Max_speed_x)
				data.vspeed.x += 0.03;

			if (data.vspeed.x < Max_speed_y)
				data.vspeed.y += 0.01;

			data.pos += data.vspeed;
		}
	}
}

void ZakoEnemy_Flower::Move3()
{
	if (data.time <= data.param.stop_time && data.vspeed.y > 0.)
	{
		data.vspeed.y -= brake;
		data.vspeed.y = std::max(0., data.vspeed.y);

		data.pos += data.vspeed;
		radAng += addAng;
	}

	if (data.time > data.param.stop_time && isReturn == false)
	{
		if (data.time == data.param.stop_time + 1)
		{
			data.vspeed.SetZero();
			addAng = 0;
		}

		addAng += 0.005;
		radAng += addAng;
		if (addAng >= 0.5)
			isReturn = true;
	}

	if (isReturn)
	{
		addAng -= 0.004;
		addAng = std::max(0., addAng);
		radAng += addAng;

		if (data.time >= data.param.out_time)
		{
			if (data.vspeed.x < Max_speed_x)
				data.vspeed.x -= 0.03;

			if (data.vspeed.x < Max_speed_y)
				data.vspeed.y += 0.01;

			data.pos += data.vspeed;
		}
	}
}

void ZakoEnemy_Flower::Fire0()
{
	if (isReturn && hasMissile)
	{
		for (int i = 0; i < 3; ++i)
		{
			Vector2D dir = Vector2D::GetVec((GetRand(20) - 10) / 10., (GetRand(20) - 10) / 10.);
			Vector2D force = dir.Normalize() * 7;
			IEneShot::Fire_Ang(eShotType::missile, data.pos, 0, 4, dir.ToRad(), 1.01, 2, eShotAI::homing);
		}
		Effector::PlaySpread(data.pos.x, data.pos.y, GetRand(100), eSpread_SmallBlue);
		hasMissile = false;
	}
}


// ==========================================================Flower
