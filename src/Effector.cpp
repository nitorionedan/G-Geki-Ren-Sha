#include "Effector.hpp"
#include "Effect.hpp"
#include "PieceEffect.hpp"
#include "Explosion.hpp"
#include "ExplosionEffect.hpp"
#include "ExplosionS.hpp"
#include "Piece.hpp"
#include "ChargeEffect.hpp"
#include <DxLib.h>

constexpr int EX_NUM = 4;

PieceEffect*	Effector::pieceef;
Effect**		Effector::effect;
Effector::Shock Effector::s_shock[10];
SmokeEffect* Effector::smoke;


Effector::Effector()
{
	img_shock = LoadGraph("GRAPH/GAME/EFFECT/circle.png");

	pieceef = new PieceEffect;

	effect = new Effect*[EX_NUM];
	effect[0] = new Effect(new ExplosionEffect(eExplosion_small));
	effect[1] = new Effect(new ExplosionEffect(eExplosion_normal));
	effect[2] = new Effect(new ExplosionEffect(eExplosion_big));
	effect[3] = new Effect(new ExplosionEffect(eExplosion_long));
	smoke = new SmokeEffect;

	for (auto& i : s_shock)
	{
		i.img = img_shock;
		i.isPlay = false;
		i.x = 0.;
		i.y = 0.;
		i.exrate = 1.;
	}
}


Effector::~Effector()
{
	DeleteGraph(img_shock);

	delete pieceef;
	for (int i = 0; i < EX_NUM; i++)
		delete effect[i];
	delete[] effect;
	delete smoke;
}


void Effector::Update()
{
	pieceef->Update();
	
	for (int i = 0; i < EX_NUM; i++)
		effect[i]->Update();
	
	for (auto& i : s_shock)
	{
		if (i.isPlay == false)
			continue;
		i.exrate += 0.5;
		if (i.exrate > 30)
		{
			i.isPlay = false;
			i.exrate = 1.;
		}
	}

	smoke->Update();
}


void Effector::Draw()
{
	pieceef->Draw();

	for (int i = 0; i < EX_NUM; i++)
		effect[i]->Draw();

	smoke->Draw();
}


void Effector::Draw_Shock()
{
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	for (auto i : s_shock)
	{
		if (i.isPlay == false)
			continue;

		int alpha = 200 - i.exrate * i.exrate;
		if (alpha < 0)
			alpha = 0;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawRotaGraphF(i.x, i.y, i.exrate, 0., i.img, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	SetDrawMode(DX_DRAWMODE_NEAREST);
}


void Effector::PlayAnime(const double& PlayX, const double& PlayY, eExplosion_t type)
{
	switch (type)
	{
	case eExplosion_small:
		effect[0]->PlayAnime(PlayX, PlayY);	break;
	case eExplosion_normal:
		effect[1]->PlayAnime(PlayX, PlayY);	break;
	case eExplosion_big:
		effect[1]->PlayAnime(PlayX, PlayY);
		effect[2]->PlayAnime(PlayX, PlayY);	break;
	case eExplosion_long:
		effect[3]->PlayAnime(PlayX, PlayY);	break;
	}
}


void Effector::PlaySpread(const double & PosX, const double & PosY, const double & ANGLE, eSpread_t type) {
	pieceef->PlayAnime(PosX, PosY, ANGLE, type);
}


void Effector::PlayShock(double x, double y)
{
	for(auto& i : s_shock)
	{
		if (i.isPlay)
			continue;
		i.isPlay = true;
		i.x = x;
		i.y = y;
		break;
	}
}


void Effector::PlaySmoke(double x, double y, eSmokeColor colorType) {
	smoke->PlayAnime(x, y, colorType);
}
