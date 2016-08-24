#include "DxLib.h"

#include "EShot02.hpp"
#include "DebugMode.hpp"
#include "Game.hpp"

#include <cmath>


EShot02::EShot02(int colorNum)
	: HIT_RANGE(7)
{
	switch (colorNum)
	{
	case 0:	gh = LoadGraph("GRAPH/GAME/Eshot/enaga0.png");	break;
	case 1:	gh = LoadGraph("GRAPH/GAME/Eshot/enaga1.png");	break;
	case 2:	gh = LoadGraph("GRAPH/GAME/Eshot/enaga2.png");	break;
	case 3:	gh = LoadGraph("GRAPH/GAME/Eshot/enaga3.png");	break;
	case 4:	gh = LoadGraph("GRAPH/GAME/Eshot/enaga4.png");	break;
	default:	gh = LoadGraph("GRAPH/GAME/Eshot/eshot03.png");	break;
	}
}


EShot02::~EShot02()
{
	DeleteGraph(gh);
}


void EShot02::Update(const double & PosX, const double & PosY)
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
}


void EShot02::Draw()
{
	for (int i = 0; i < ALL_NUM; i++)
	{
		if (!isExist[i])	continue;

		// アニメーション
		DrawRotaGraph(pos[i].x, pos[i].y, 2.0, vangle[i] - DX_PI / 2, gh, true);

		if (!DebugMode::isTest)	continue;

		DrawCircle(pos[i].x, pos[i].y, HIT_RANGE, GetColor(255, 0, 0), FALSE);
	}
}


void EShot02::Fire(const double & SPEED, const double & ANGLE)
{
	for (int i = 0; i < ALL_NUM; i++)
	{
		if (isExist[i])	continue;

		isExist[i] = true;
		rad[i] = std::atan2(Game::GetPlayerPos().y - pos[i].y, Game::GetPlayerPos().x - pos[i].x) - DX_PI / 2;
		vspeed[i] = SPEED;
		vangle[i] = ANGLE;	break;
	}
}


void EShot02::Fire(const double & PosX, const double & PosY, const double & SPEED, const double & ANGLE)
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


bool EShot02::IsHit(const double & ColX, const double & ColY, const double & ColR)
{
	const bool& IS_HIT = Vector2D::CirclesCollision(HIT_RANGE, ColR, pos[0].x, pos[0].y, ColX, ColY + 9.);

	return IS_HIT;
}


void EShot02::Move(const int & id)
{
	// 進ませる
	pos[id].x += vspeed[id] * std::cos(vangle[id]);
	pos[id].y += vspeed[id] * std::sin(vangle[id]);

	// 当たり判定チェック
	const bool& IS_HIT = Game::IsHitPlayer(Player::HIT_RANGE, HIT_RANGE,
		Game::GetPlayerPos().x, Game::GetPlayerPos().y, pos[id].x, pos[id].y);

	// 当たったら消す
	if (IS_HIT)	isExist[id] = false;

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