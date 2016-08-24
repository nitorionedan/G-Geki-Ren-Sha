#include "DxLib.h"

#include "Bomb.hpp"
#include "Player.hpp"
#include "Game.hpp"
#include "Keyboard.hpp"
#include "EnemyMng.hpp"
#include "Score.hpp"


static const int EFFECT_NUM = 10;

BombEffect Bomb::effect[EFFECT_NUM];


Bomb::Bomb()
	: time(0)
	, isBomb(false)
{
	sh = LoadSoundMem("SOUND/SE/danmatu.wav");

	for (int i = 0; i < EFFECT_NUM; i++)
	{
		effect[i].rad = 0;
		effect[i].isExist = false;
	}
}


Bomb::~Bomb()
{
}


void Bomb::Update()
{
	// 波紋の動き
	MoveEffect();

	if (!isBomb)	return;

	if (time % 10 == 0)
	{

		/// 敵にダメージ
	}

	// 波紋発動
	if (time % 20 == 0)	PlayEffect();

	time--;

	// 終わる少し前にボムOK
	if (time == 20)	isBomb = false;
}


void Bomb::Draw()
{
	DrawEffect();

	if (!isBomb)	return;
}


void Bomb::Fire(int shiftLevel)
{
	if (isBomb) return;
	if (Game::GetPlayerBomb() == 0)	return;

	isBomb = true;
	Game::DownBombNum();
	Game::ShiftReset();

	switch (shiftLevel)
	{
	case 0:
		time = 60;
		break;
	case 1:
		time = 80;
		break;
	case 2:
		time = 100;
		break;
	case 3:
		time = 120;
		break;
	case 4:
		time = 160;
		break;

	default:
		break;
	}

	PlaySoundMem(sh, DX_PLAYTYPE_BACK);
}


bool Bomb::IsHit(const int & ColCircle, const double & ColX, const double & ColY)
{
	bool isHit = false;

	for (int i = 0; i < EFFECT_NUM; i++)
	{
		if (!effect[i].isExist)	continue;

		isHit = Vector2D::CirclesCollision(ColCircle, effect[i].rad, ColX, ColY, 320., 240.);
		
		if (isHit)	Score::AddScore(50);
		if (isHit)	return isHit;
	}

	return isHit;
}


void Bomb::DrawEffect()
{
	for (auto& ary : effect)
	{
		if (ary.isExist == false) continue;

		DrawCircle(320, 240, ary.rad, GetColor(255, 255, 255), false);
	}

	for (int i = 0; i < EFFECT_NUM; i++)
	{
		if (!effect[i].isExist)	continue;

		DrawCircle(320, 240, effect[i].rad, GetColor(255, 255, 255), false);
	}
}


void Bomb::PlayEffect()
{
	for (int i = 0; i < EFFECT_NUM; i++)
	{
		if (effect[i].isExist)	continue;

		effect[i].isExist = true;
		effect[i].rad = 0;
		break;
	}
}


void Bomb::MoveEffect()
{
	for (int i = 0; i < EFFECT_NUM; i++)
	{
		if (!effect[i].isExist) continue;

		effect[i].rad += 5;

		EnemyMng::IsHit(effect[i].rad, 320., 240., 100);

		// リセット
		if (effect[i].rad > 400)
		{
			effect[i].isExist = false;
		}
	}
}

// EOF