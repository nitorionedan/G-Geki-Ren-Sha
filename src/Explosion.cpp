#include "DxLib.h"

#include "Explosion.hpp"


Explosion::Explosion(eEpl_tlong tlong, bool isRed)
{
	c_play = new Counter*[COUNTER_NUM];

	switch (tlong)
	{
	case eEpl_short:
		for (int i = 0; i < COUNTER_NUM; i++)
			c_play[i] = new Counter(44);	break;
	case eEpl_normal:
		for (int i = 0; i < COUNTER_NUM; i++)
			c_play[i] = new Counter(61);	break;
	case eEpl_long:
		for (int i = 0; i < COUNTER_NUM; i++)
			c_play[i] = new Counter(91);	break;
	}
	
	kind = tlong;

	if (!isRed) LoadDivGraph("GRAPH/GAME/EFFECT/explosion01.png", 8, 4, 2, 20, 20, gh_explo);
	if (isRed)	LoadDivGraph("GRAPH/GAME/EFFECT/explosion02.png", 8, 4, 2, 20, 20, gh_explo);
	
	for (auto &flag : isPlay) flag = false;
}


Explosion::~Explosion()
{
	for (int i = 0; i < COUNTER_NUM; ++i) delete c_play[i];
	delete[] c_play;

	for (int i = 0; i < sizeof(gh_explo) / sizeof(gh_explo[0]); i++) DeleteGraph(gh_explo[i]);
}


void Explosion::Update()
{
	for (int i = 0; i != isPlay.size(); ++i)
	{
		if (isPlay[i] == false) continue;

		c_play[i]->Update();

		pos[i].AddVec(speed[i]);
		
		const double BRAKE = 0.2; // ブレーキ…

		if(c_play[i]->isLast())
		{
			c_play[i]->Reset();
			isPlay[i] = false;
		}
	}

}


void Explosion::Draw()
{
	for (int i = 0; i != isPlay.size(); ++i)
	{
		if (isPlay[i] == false) continue;

		switch (kind)
		{
		case eEpl_short:
			if (c_play[i]->isEach(40, 43))
				DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_explo[0], true);
			if (c_play[i]->isEach(36, 39))
				DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_explo[1], true);
			if (c_play[i]->isEach(32, 35))
				DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_explo[2], true);
			if (c_play[i]->isEach(28, 31))
				DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_explo[3], true);
			if (c_play[i]->isEach(24, 27))
				DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_explo[4], true);
			if (c_play[i]->isEach(20, 23))
				DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_explo[5], true);
			if (c_play[i]->isEach(5, 19))
				DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_explo[6], true);
			if (c_play[i]->isEach(1, 4))
				DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_explo[7], true);
			break;
		case eEpl_normal:
			if (c_play[i]->isEach(57, 60))
				DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_explo[0], true);
			if (c_play[i]->isEach(53, 56))
				DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_explo[1], true);
			if (c_play[i]->isEach(45, 52))
				DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_explo[2], true);
			if (c_play[i]->isEach(37, 44))
				DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_explo[3], true);
			if (c_play[i]->isEach(29, 36))
				DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_explo[4], true);
			if (c_play[i]->isEach(21, 28))
				DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_explo[5], true);
			if (c_play[i]->isEach(13, 20))
				DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_explo[6], true);
			if (c_play[i]->isEach(1, 12))
				DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_explo[7], true);
			break;
		case eEpl_long:
			break;
		}
	}
}


void Explosion::DrawTest()
{
//	DrawGraph(0, 0, gh_explo[1], true);

	for (int i = 0; i < 3; i++)
		DrawFormatString(130, 20 + (i * 20), GetColor(0, 255, 0), "c[%d] = %d", i, c_play[i]->GetNowcount());
	DrawFormatString(100, 100, GetColor(0, 255, 0), "speed[0].x = %lf", speed[0].x);
	DrawFormatString(100, 120, GetColor(0, 255, 0), "speed[0].x = %lf", speed[0].y);
}


void Explosion::PlayAnime(const double & PlayX, const double & PlayY, const double& SpeedX, const double& SpeedY)
{
	for (int i = 0; i != isPlay.size(); ++i)
	{
		if (isPlay[i] == true) continue;
		isPlay[i] = true;
		pos[i].SetVecor2D(PlayX, PlayY);
		speed[i].SetVecor2D(SpeedX, SpeedY);

		// ランダムに角度を決める
		int rand = GetRand(3);
		switch (rand)
		{
		case 0: angle[i] = 0. * DX_PI / 180.;	break;
		case 1: angle[i] = 90. * DX_PI / 180.;	break;
		case 2: angle[i] = 180. * DX_PI / 180.;	break;
		case 3: angle[i] = 270. * DX_PI / 180.;	break;
		}

		break;
	}
}