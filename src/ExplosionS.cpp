#include "DxLib.h"

#include "ExplosionS.hpp"


ExplosionS::ExplosionS(eEpl_tlong type_)
	: type(type_)
{
	explosion  = new Explosion(type_, false);
	explosion2 = new Explosion(type_, true);
	c_play = new Counter*[EXPLOSION_S_NUM];
	switch (type)
	{
	case eEpl_short:
		for (int i = 0; i < EXPLOSION_S_NUM; i++)
			c_play[i] = new Counter(17);	
		break;
	case eEpl_normal:
		for (int i = 0; i < EXPLOSION_S_NUM; i++)
			c_play[i] = new Counter(31);
		break;
	case eEpl_long:
		for (int i = 0; i < EXPLOSION_S_NUM; i++)
			c_play[i] = new Counter(31);
		break;
	}

	for (auto &vec : pos) vec.SetZero();
	for (bool &flag : isPlay) flag = false;
}


ExplosionS::~ExplosionS()
{
	delete explosion;
	delete explosion2;
	for (int i = 0; i < EXPLOSION_S_NUM; ++i)	delete c_play[i];
	delete[] c_play;
}


void ExplosionS::Update()
{
	// アニメ再生
	for (int i = 0; i != isPlay.size(); ++i)
	{
		if (isPlay[i] == false) continue;

		c_play[i]->Update();

		// アニメ開始
		Fire(i);

		if (c_play[i]->isLast())	isPlay[i] = false;
		if(c_play[i]->isLast())		c_play[i]->Reset();
	}
	Move();
	explosion->Update();
	explosion2->Update();
}


void ExplosionS::Draw()
{
	explosion->Draw();
	explosion2->Draw();
}


void ExplosionS::Fire(const int& INDEX)
{
	Vector2D V, cV;

	switch (type)
	{
	case eEpl_short:
		// 2フレームに１度、爆発１個
		if (c_play[INDEX]->Remainder(2) != 0)	break;
		
		// 爆風の速さを決定
		if (c_play[INDEX]->isEach(9, 16))
			V.SetVecor2D((GetRand(20) - 10.) / 10., (GetRand(20) - 10.) / 10.);
		if (c_play[INDEX]->isEach(1, 8))
			V.SetVecor2D((GetRand(16) - 8.) / 10., (GetRand(16) - 8.) / 10.);

		// 爆発する位置をランダムにずらす
		cV.SetVecor2D(pos[INDEX].x + GetRand(40) - 20, pos[INDEX].y + GetRand(40) - 20);
		break;
	case eEpl_normal:
		if(c_play[INDEX]->isEach(16, 30))
			V.SetVecor2D((GetRand(40) - 20.) / 10., (GetRand(40) - 20.) / 10.);
		if(c_play[INDEX]->isEach(1, 15))
			V.SetVecor2D((GetRand(20) - 10.) / 10., (GetRand(20) - 10.) / 10.);
		cV.SetVecor2D(pos[INDEX].x + GetRand(40) - 20, pos[INDEX].y + GetRand(40) - 20);
		break;
	case eEpl_long:

		break;
	}

	// それぞれ色の違う爆風を描画
	if (!isRed[INDEX])	explosion->PlayAnime(cV.x, cV.y, V.x, V.y);
	if (isRed[INDEX])	explosion2->PlayAnime(cV.x, cV.y, V.x, V.y);
}


void ExplosionS::PlayAnime(const double & PlayX, const double & PlayY)
{
	int color;

	for (int i = 0; i != isPlay.size(); ++i)
	{
		if (isPlay[i] == true) continue;

		isPlay[i] = true;
		pos[i].SetVecor2D(PlayX, PlayY);
		
		color = GetRand(2);
		if (color == 0) isRed[i] = false;
		if (color > 0)	isRed[i] = true;
		break;
	}

}


void ExplosionS::Move()
{

}

// EOF