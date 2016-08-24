#include "DxLib.h"

#include "ExplosionEffect.hpp"


ExplosionEffect::ExplosionEffect(eExplosion_t kind_)
	: kind(kind_)
{
	c_play = new Counter*[EXEF_ALL_NUM];

	switch (kind)
	{
	case eExplosion_small:
		mExplosionS = new ExplosionS(eEpl_short);
		for (int i = 0; i < EXEF_ALL_NUM; i++)
			c_play[i] = new Counter(2);
		sh_ex = LoadSoundMem("SOUND/SE/explosion06.wav");
//		sh_ex = LoadSoundMem("SOUND/SE/smallEx00.wav");
		break;

	case eExplosion_normal:
		mExplosionS = new ExplosionS(eEpl_normal);
		for (int i = 0; i < EXEF_ALL_NUM; i++)
			c_play[i] = new Counter(41);
		sh_ex = LoadSoundMem("SOUND/SE/explosion04.mp3");
		break;

	case eExplosion_big:
		mExplosionS = new ExplosionS(eEpl_long);
		for (int i = 0; i < EXEF_ALL_NUM; i++)
			c_play[i] = new Counter(41);
		sh_ex = LoadSoundMem("SOUND/SE/explosion03.mp3");
		break;

	case eExplosion_long:
		mExplosionS = new ExplosionS(eEpl_normal);
		for (int i = 0; i < EXEF_ALL_NUM; i++)
			c_play[i] = new Counter(41);
		sh_ex = LoadSoundMem("SOUND/SE/explosion04.mp3");
		break;
	}

	for (auto &p : pos)				p.SetZero();
	for (bool &flag : isPlay)		flag = false;
}


ExplosionEffect::~ExplosionEffect()
{
	delete mExplosionS;
	for (int i = 0; i < EXEF_ALL_NUM; i++)	delete c_play[i];
	delete[] c_play;
	DeleteSoundMem(sh_ex);
}


void ExplosionEffect::Update()
{
	for (int i = 0; i != isPlay.size(); ++i)
	{
		if (isPlay[i] == false) continue;
		c_play[i]->Update();

		Play(i);

		if(c_play[i]->isLast())
		{
			c_play[i]->Reset();
			isPlay[i] = false;
		}
	}

	mExplosionS->Update();
}


void ExplosionEffect::Draw(){
	mExplosionS->Draw();
}


void ExplosionEffect::PlayAnime(const double & PlayX, const double & PlayY)
{
	for (int i = 0; i != isPlay.size(); ++i)
	{
		if (isPlay[i] == true) continue;
		isPlay[i] = true;
		pos[i].SetVecor2D(PlayX, PlayY);
		break;
	}
}

void ExplosionEffect::Play(const int & INDEX)
{
	Vector2D vRand;
	int nRand;

	switch (kind)
	{
	case eExplosion_small:
		mExplosionS->PlayAnime(pos[INDEX].x, pos[INDEX].y);
		PlaySoundMem(sh_ex, DX_PLAYTYPE_BACK);
		break;

	case eExplosion_normal:
		// 10フレームに１度
		if (c_play[INDEX]->Remainder(10) != 0)	break;

		// 最初の１個はずらさない
		if (c_play[INDEX]->GetNowcount() == 40)
			vRand.SetZero();

		// 位置をランダムにずらす
		if (c_play[INDEX]->GetNowcount() != 40)
		{
			nRand = GetRand(3);
			switch (nRand)
			{
			case 0:
				vRand.x = (double)GetRand(60);
				vRand.y = (double)GetRand(60);
				break;
			case 1:
				vRand.x = (double)GetRand(60);
				vRand.y = (double)GetRand(60) - 120.;
				break;
			case 2:
				vRand.x = (double)GetRand(60) - 120.;
				vRand.y = (double)GetRand(60) - 120.;
				break;
			case 3:
				vRand.x = (double)GetRand(60) - 120.;
				vRand.y = (double)GetRand(60);
				break;
			}
		}

		mExplosionS->PlayAnime(pos[INDEX].x + vRand.x, pos[INDEX].y + vRand.y);
		
		if (c_play[INDEX]->GetNowcount() == 40 || c_play[INDEX]->GetNowcount() == 10)
			PlaySoundMem(sh_ex, DX_PLAYTYPE_BACK);
		
		break;

	case eExplosion_big:
		// nanika
		mExplosionS->PlayAnime(pos[INDEX].x, pos[INDEX].y); // 40fpsの内毎フレームnormalを呼ぶw
		PlaySoundMem(sh_ex, DX_PLAYTYPE_BACK);
		break;

	case eExplosion_long:
		mExplosionS->PlayAnime(pos[INDEX].x, pos[INDEX].y);
		PlaySoundMem(sh_ex, DX_PLAYTYPE_BACK);

		break;
	}
}
