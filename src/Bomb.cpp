
#include <DxLib.h>
#include "Bomb.hpp"
#include "EnemyMng.hpp"
#include <cassert>


Bomb::Bomb()
	: time(0)
	, isBomb(false)
{
	sh = LoadSoundMem("SOUND/SE/danmatu.wav");

	pos = Vector2D::ZERO;
	for (int i = 0; i < EFFECT_NUM; i++)
	{
		effect[i].rad = 0;
		effect[i].isExist = false;
	}
	shiftLevel = 0;
}


void Bomb::Update()
{
	// ”g–ä‚Ì“®‚«
	MoveEffect();

	if (!isBomb) return;

	if (time % 10 == 0)
	{

		/// “G‚Éƒ_ƒ[ƒW
	}

	// ”g–ä”­“®
	if (time % 20 == 0)	PlayEffect();

	time--;

	// I‚í‚é­‚µ‘O‚Éƒ{ƒ€OK
	if (time == 20)	isBomb = false;
}


void Bomb::Draw()
{
	DrawEffect();
}


void Bomb::SetParam(Vector2D& pos, int& shiftLevel, int& bombNum)
{
	// TODO: remove this func
	this->shiftLevel = shiftLevel;
	this->bombNum = bombNum;
}


void Bomb::Fire()
{
	if (isBomb) return;
	if (bombNum == 0)	return;

	isBomb = true;
	pos = IPlayer::GetPos();

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

	for (int i = 0; i < _countof(effect); i++)
	{
		if (!effect[i].isExist)	continue;

		isHit = Vector2D::CirclesCollision(ColCircle, effect[i].rad, ColX, ColY, pos.x, pos.y);
		
		if (isHit)	return isHit;
	}

	return isHit;
}


void Bomb::DrawEffect()
{
	for (const auto& ary : effect)
	{
		if (ary.isExist == false) continue;
		DrawCircle(pos.x, pos.y, ary.rad, GetColor(255, 255, 255), false);
	}

	for (int i = 0; i < EFFECT_NUM; i++)
	{
		if (!effect[i].isExist)	continue;
		DrawCircle(pos.x, pos.y, effect[i].rad, GetColor(255, 255, 255), false);
	}
}


void Bomb::PlayEffect()
{
	for (int i = 0; i < _countof(effect); i++)
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

		IEnemyMng::IsHit(effect[i].rad, pos.x, pos.y, 100);

		// reset
		if (effect[i].rad > 400)
			effect[i].isExist = false;
	}
}


// ============================================================
std::shared_ptr<Bomb> IBomb::mBomb;


void IBomb::set(std::shared_ptr<Bomb> bomb)
{
	mBomb = bomb;
	assert(mBomb != nullptr && "IBomb::set()");
}


void IBomb::SetParam(Vector2D & pos, int & shiftLevel, int & bombNum){
	mBomb->SetParam(pos, shiftLevel, bombNum);
}
