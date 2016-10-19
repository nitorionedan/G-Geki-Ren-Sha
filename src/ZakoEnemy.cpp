#include "ZakoEnemy.hpp"
#include "Graphics2D.hpp"
#include "Stage.hpp"
#include "IScore.hpp"
#include "Keyboard.hpp"
#include "Player.hpp"
#include <DxLib.h>
#include <cassert>
#include <cmath>

namespace
{
	constexpr int SCREEN_LIMIT_XL = -10;
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

// ==========================================================Ze
ZakoEnemy_Ze::ZakoEnemy_Ze(const tEnemyData & data)
	: Speed(4)
	, Brake(0.03)
	, Easing(0.99999999)
{
	gh = LoadGraph("GRAPH/GAME/ENEMY/ene00.png");
	this->data.param = data;
	this->data.rad = 0;
	this->data.pos.SetVec(data.x_pos, data.y_pos);
	this->data.vspeed.SetVec(2., 4.);
	this->data.hitRange = 20;
	this->data.isExist = false;
	this->data.isUngry = false;
	this->data.isHit = false;
	this->data.isMove = false;
}

ZakoEnemy_Ze::~ZakoEnemy_Ze()
{
	DeleteGraph(gh);
}

void ZakoEnemy_Ze::Update()
{
	++data.time;

	// 登場時間が来たら出てくる
	if (IStage::GetTime() == data.param.in_time)
		data.isExist = true;

	if (data.isExist == false)
		return;

	data.isHit = false;

	// 攻撃されたら怒る
	if (Keyboard::Instance()->isPush(KEY_INPUT_Z))
		data.isUngry = true;

	/* Move */
	switch (data.param.m_pattern)
	{
	case 0: 
		Move0();
		break;
	default: assert(!"out of range");
	}
}

void ZakoEnemy_Ze::Draw()
{
	DrawRotaGraph(data.pos.x, data.pos.y, 2., data.rad, gh, true);
}

void ZakoEnemy_Ze::Move0()
{
	const bool& IS_IN = (data.time >= 0 && data.time <= data.param.stop_time);
	const bool& IS_OUT = (data.time >= data.param.out_time);

	// 最初の移動
	if (IS_IN)
	{
		// プレイヤーの前か画面の上半分なら
		if (IPlayer::GetPos().y - 60.0 > data.pos.y || data.pos.y >= 320.)
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
			data.vspeed.x *= 0.99999999;
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

	// 帰る（笑）
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

	// 退場
	if (IS_OUT)
	{
		if (data.pos.y > -40)
			return;
		data.isExist = false;
		IEnemyMng::CountDownEneNum();
	}
}

void ZakoEnemy_Ze::HitCheck()
{
}


// ==========================================================Career
ZakoEnemy_Career::ZakoEnemy_Career(const tEnemyData & data)
{
	LoadDivGraph("GRAPH/GAME/ENEMY/career.png", _countof(gh), 4, 2, 124, 78, gh);
	this->data.param = data;
	this->data.rad = 0;
	this->data.pos.SetVec(data.x_pos, data.y_pos);
	this->data.hitRange = 50;
	this->data.isExist = false;
	this->data.isUngry = false;
	this->data.isHit = false;
	this->data.isMove = false;
}

ZakoEnemy_Career::~ZakoEnemy_Career()
{
	for (auto i : gh)
		DeleteGraph(i);
}

void ZakoEnemy_Career::Update()
{
	++data.time;

}

void ZakoEnemy_Career::Draw()
{
	DrawAnime(data.pos.x, data.pos.y, 2., 0., data.time, _countof(gh), 8, gh);
}

void ZakoEnemy_Career::HitCheck()
{
}


// ==========================================================Raide
ZakoEnemy_Den::ZakoEnemy_Den(const tEnemyData & data)
{
	LoadDivGraph("GRAPH/GAME/ENEMY/ene02.png", _countof(gh), 4, 1, 31, 16, gh);
	this->data.param = data;
	this->data.rad = 0;
	this->data.pos.SetVec(data.x_pos, data.y_pos);
	this->data.vspeed.SetVec(2., 0.1);
	this->data.hitRange = 20;
	this->data.isExist = false;
	this->data.isUngry = false;
	this->data.isHit = false;
	this->data.isMove = false;
}

ZakoEnemy_Den::~ZakoEnemy_Den()
{
	for (auto i : gh)
		DeleteGraph(i);
}

void ZakoEnemy_Den::Update()
{
	++data.time;

	static float c_move = 0.f;
	c_move += 0.01f;

	data.vspeed.SetVec(0.7, 0.2);

	if (s_time < data.param.stop_time ||
		s_time > data.param.stop_time + 40)
	{
		data.pos.y += data.vspeed.y;
	}

	data.pos.x += data.vspeed.x * std::cos(c_move);

	if (data.pos.y > SCREEN_LIMIT_YB)
	{
		data.isExist = false;
		IEnemyMng::CountDownEneNum();
	}

}

void ZakoEnemy_Den::Draw()
{
	DrawAnime(data.pos.x, data.pos.y, 2., data.rad, data.time, _countof(gh), 2, gh);
}

void ZakoEnemy_Den::HitCheck()
{
}


// ==========================================================
