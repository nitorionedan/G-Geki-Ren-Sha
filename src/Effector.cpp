#include "Effector.hpp"


PieceEffect*	Effector::pieceef;
Effect**		Effector::effect;


Effector::Effector()
{
	pieceef = new PieceEffect;

	effect = new Effect*[EX_NUM];
	effect[0] = new Effect(new ExplosionEffect(eExplosion_small));
	effect[1] = new Effect(new ExplosionEffect(eExplosion_normal));
	effect[2] = new Effect(new ExplosionEffect(eExplosion_big));
	effect[3] = new Effect(new ExplosionEffect(eExplosion_long));
}


Effector::~Effector()
{
	delete pieceef;
	{
		for (int i = 0; i < EX_NUM; i++)	delete effect[i];
		delete[] effect;
	}
}


void Effector::Update()
{
	pieceef->Update();
	for (int i = 0; i < EX_NUM; i++)	effect[i]->Update();
}


void Effector::Draw()
{
	pieceef->Draw();

	for (int i = 0; i < EX_NUM; i++)
		effect[i]->Draw();
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