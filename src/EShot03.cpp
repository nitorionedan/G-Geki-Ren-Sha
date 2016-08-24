#include "DxLib.h"

#include "EShot03.hpp"
#include "DebugMode.hpp"
#include "Game.hpp"
#include "HitEffect.hpp"
#include "Graphics2D.hpp"

#include <cmath>


EShot03::EShot03()
	: ALL_FRAME_NUM(sizeof(gh) / sizeof(gh[0]))
	, FRAME_TIME(2)
	, HIT_RANGE(50)
	, effect(new Effect(new HitEffect))
{
	time.fill(0);
	rad.fill(0.);
	ang.fill(0.);
	isExist.fill(false);
	LoadDivGraph("GRAPH/GAME/Eshot/bigshot.png", 4, 4, 1, 60, 60, gh);
}


EShot03::~EShot03()
{
	for (int i = 0; i < sizeof(gh) / sizeof(gh[0]); i++)
		DeleteGraph(gh[i]);
}


void EShot03::Update(const double & PosX, const double & PosY)
{
	for (int i = 0; i < ALL_NUM; i++)
	{
		if (!isExist[i])
		{
			pos[i].SetVecor2D(PosX, PosY);	 continue;
		}

		time[i]++;
		ang[i] -= 0.2;
		Move(i);
	}

	effect->Update();
}


void EShot03::Draw()
{
	for (int i = 0; i < ALL_NUM; i++)
	{
		if (!isExist[i])	continue;

		// アニメーション
		DrawAnime(pos[i].x, pos[i].y, ang[i], time[i], ALL_FRAME_NUM, FRAME_TIME, gh);

		if (!DebugMode::isTest)	continue;

		DrawCircle(pos[i].x, pos[i].y, HIT_RANGE, GetColor(255, 0, 0), false);
	}

	effect->Draw();
}


void EShot03::Fire(const double & SPEED, const double & ANGLE)
{
	for (int i = 0; i < ALL_NUM; i++)
	{
		if (isExist[i])	continue;

		isExist[i] = true;
		vspeed[i] = SPEED;
		vangle[i] = ANGLE;	break;
	}
}


void EShot03::Fire(const double & PosX, const double & PosY, const double & SPEED, const double & ANGLE)
{
	for (int i = 0; i < ALL_NUM; i++)
	{
		if (isExist[i])	continue;

		isExist[i] = true;
		pos[i].SetVecor2D(PosX, PosY);
		vspeed[i] = SPEED;
		vangle[i] = ANGLE;	break;
	}
}


bool EShot03::IsHit(const double & ColX, const double & ColY, const double & ColR)
{
	const bool& IS_HIT = Vector2D::CirclesCollision(HIT_RANGE, ColR, pos[0].x, pos[0].y, ColX, ColY + 9.);

	return IS_HIT;
}


void EShot03::Move(const int & id)
{
	// 進ませる
	pos[id].x += vspeed[id] * std::cos(vangle[id]);
	pos[id].y += vspeed[id] * std::sin(vangle[id]);

	// 当たり判定チェック
	const bool& IS_HIT = Game::IsHitPlayer(Player::HIT_RANGE, HIT_RANGE,
		Game::GetPlayerPos().x, Game::GetPlayerPos().y, pos[id].x, pos[id].y);

	const bool& IS_HIT2 = Bomb::IsHit(HIT_RANGE, pos[id].x, pos[id].y);

	// 当たっら爆発
	if (IS_HIT)	Game::PlayAnime(pos[id].x, pos[id].y, eExplosion_small);

	// 画面外で消す
	if (Shot::SC_LIMIT_XL > pos[id].x ||
		Shot::SC_LIMIT_XR < pos[id].x ||
		Shot::SC_LIMIT_YT > pos[id].y ||
		Shot::SC_LIMIT_YB < pos[id].y)
	{
		isExist[id] = false;
	}
}

// EOF