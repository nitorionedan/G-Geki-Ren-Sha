#include "DxLib.h"

#include "EShot00.hpp"
#include "Graphics2D.hpp"
#include "Game.hpp"
#include "HitEffect.hpp"
#include "Bomb.hpp"

#include <cmath>


EShot00::EShot00()
	: ALL_FRAME_NUM(sizeof(gh) / sizeof(gh[0]))
	, FRAME_TIME(2)
	, ALL_FRAME_TIME(ALL_FRAME_NUM * FRAME_TIME + 1) // 余りをもとめるため、１を足して調整
	, HIT_RANGE(7)
	, effect(new Effect(new HitEffect))
{
	time.fill(0);
	rad.fill(0.);
	isExist.fill(false);
	LoadDivGraph("GRAPH/GAME/Eshot/eshot00.png", 12, 4, 3, 8, 8, gh);
}


EShot00::~EShot00()
{
	for (int i = 0; i < sizeof(gh) / sizeof(gh[0]); i++)	DeleteGraph(gh[i]);
}


void EShot00::Update(const double& PosX, const double& PosY)
{
	for (int i = 0; i < ALL_NUM; i++)
	{
		if (!isExist[i])
		{
			pos[i].SetVecor2D(PosX, PosY);	 continue;
		}
	
		time[i]++;
		Move(i);
	}

	effect->Update();
}


void EShot00::Draw()
{
	for (int i = 0; i < ALL_NUM; i++)
	{
		if (!isExist[i])	continue;

		// アニメーション
		DrawAnime(pos[i].x, pos[i].y, vangle[i], time[i], ALL_FRAME_NUM, FRAME_TIME, gh);
	}

	effect->Draw();
}


void EShot00::Fire(const double& SPEED, const double& ANGLE)
{
	for (int i = 0; i < ALL_NUM; i++)
	{
		if (isExist[i])	continue;

		isExist[i] = true;
		vspeed[i] = SPEED;
		vangle[i] = ANGLE;	break;
	}
}


void EShot00::Fire(const double & PosX, const double & PosY, const double & SPEED, const double & ANGLE)
{
	for (int i = 0; i < ALL_NUM; i++)
	{
		if (isExist[i])	continue;

		isExist[i] = true;
		pos[i].SetVecor2D(PosX, PosY);	 continue;
		vspeed[i] = SPEED;
		vangle[i] = ANGLE;	break;
	}
}


void EShot00::Move(const int & id)
{
	// 進ませる
	pos[id].x += vspeed[id] * std::cos(vangle[id]);
	pos[id].y += vspeed[id] * std::sin(vangle[id]);

	// 当たり判定チェック
	const bool& IS_HIT = Game::IsHitPlayer(Player::HIT_RANGE, HIT_RANGE,
											Game::GetPlayerPos().x, Game::GetPlayerPos().y, pos[id].x, pos[id].y);
	const bool& IS_HIT2 = Bomb::IsHit(HIT_RANGE, pos[id].x, pos[id].y);

	// 当たったら消す
	if (IS_HIT || IS_HIT2)
	{
		isExist[id] = false;
		effect->PlayAnime(pos[id].x, pos[id].y);
	}

	// 画面外で消す
	if (Shot::SC_LIMIT_XL > pos[id].x ||
		Shot::SC_LIMIT_XR < pos[id].x ||
		Shot::SC_LIMIT_YT > pos[id].y ||
		Shot::SC_LIMIT_YB < pos[id].y)
	{
		isExist[id] = false;
	}
}

/*
void EShot00::DrawAnime(const double & PosX, const double & PosY, const double & ANGLE, const int & TIME, const int& FRAME_NUM, const int & TIME_FRAME, int Gr_Handle[])
{
	const int& SUM_FRAME_TIME = (FRAME_NUM * TIME_FRAME);

	// アニメーション
	for (int j = 0; j < FRAME_NUM; j++)
	{
		if (TIME % SUM_FRAME_TIME < j * FRAME_TIME ||
			TIME % SUM_FRAME_TIME > FRAME_TIME + (j * FRAME_TIME))
			continue;

		DrawRotaGraph(PosX, PosY, 2.0, ANGLE, Gr_Handle[j], true);

		DrawCircle(PosX, PosY, HIT_RANGE, GetColor(0, 255, 0), false);
		break;
	}
}
*/

/*
void EShot00::DrawAnime(const double & PosX, const double & PosY, const double & ANGLE, const int & FRAME_NUM, const int & TIME_FRAME, int Gr_Handle[])
{
	const int& SUM_FRAME_TIME = (FRAME_NUM * TIME_FRAME);
	static int aniTime = 0;
	aniTime++;

	// アニメーション
	for (int j = 0; j < FRAME_NUM; j++)
	{
		if (aniTime % SUM_FRAME_TIME < j * FRAME_TIME ||
			aniTime % SUM_FRAME_TIME > FRAME_TIME + (j * FRAME_TIME))
			continue;

		DrawRotaGraph(PosX, PosY, 2.0, ANGLE, Gr_Handle[j], true);

		DrawCircle(PosX, PosY, HIT_RANGE, GetColor(0, 255, 0), false);
		break;
	}
}
*/


bool EShot00::IsHit(const double & ColX, const double & ColY, const double & ColR)
{
	const bool& IS_HIT = Vector2D::CirclesCollision(HIT_RANGE, ColR, pos[0].x, pos[0].y, ColX, ColY + 9.);

	return IS_HIT;
}

// EOF