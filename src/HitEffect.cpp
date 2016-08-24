#include "DxLib.h"

#include "HitEffect.hpp"


HitEffect::HitEffect()
{
	for (auto &c : c_hit) c = new Counter(16);
	for (int i = 0; i != pos.size(); ++i)	pos[i].SetVecor2D(0.0, 0.0);
	for (bool &b : isHit)	b = false;
	LoadDivGraph("GRAPH/GAME/EFFECT/hit128x32.png", 4, 4, 1, 32, 32, gh_hit);

	angle.fill(0.0);
	exrate.fill(2.0);
}


HitEffect::~HitEffect()
{
	for (auto &c : c_hit) delete c;
	for (int i = 0; i < 4; i++)
		DeleteGraph(gh_hit[i]);
}


void HitEffect::Update()
{
	for (int i = 0; i < isHit.size(); ++i)
	{
		if (isHit[i] == false) continue;

		c_hit[i]->Update();
		pos[i].y += 2.;
		
		if(c_hit[i]->isLast())
		{
			isHit[i] = false;
			c_hit[i]->Reset();
		}
	}
}


void HitEffect::Draw()
{
	for (int i = 0; i < isHit.size(); ++i)
	{
		if (isHit[i] == false)	continue;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
//		SetDrawBright(255, 0, 0);


		int count = c_hit[i]->GetNowcount();

		if (c_hit[i]->isEach(13, 15))
			DrawRotaGraph(pos[i].x, pos[i].y, exrate[i], angle[i], gh_hit[0], true);
		if (c_hit[i]->isEach(9, 12))
			DrawRotaGraph(pos[i].x, pos[i].y, exrate[i], angle[i], gh_hit[1], true);
		if (c_hit[i]->isEach(5, 8))
			DrawRotaGraph(pos[i].x, pos[i].y, exrate[i], angle[i], gh_hit[2], true);
		if (c_hit[i]->isEach(1, 4))
			DrawRotaGraph(pos[i].x, pos[i].y, exrate[i], angle[i], gh_hit[3], true);


		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
//		SetDrawBright(255, 255, 255);
	}
	// TEST
	//	DrawRotaGraph(20, 20, 2.0, 0.0, gh_hit[0], true);
	//	DrawFormatString(20, 20, GetColor(0, 255, 0), "gh->%d", gh_hit[0]);
	//	DrawGraph(0, 0, h_test, true);
	//	DrawRotaGraph(20, 20, 2.0, 0.0, h_test, true);
	/*
	for (int i = 0; i != isHit.size(); ++i)
	{
		DrawFormatString(300, 20 + (i * 20), GetColor(0, 255, 0), "c->%d", c_hit[i]->GetNowcount());
	}
	*/
}


void HitEffect::PlayAnime(const double & MyX, const double & MyY)
{
	for (int i = 0; i != isHit.size(); ++i)
	{
		if (isHit[i] == true)	continue;
		isHit[i] = true;
		pos[i].SetVecor2D(MyX, MyY);
		angle[i] = GetRand(10) / 10.0;
//		exrate[i] = (GetRand(10) / 10.0) + 1.0;
		break;
	}
}
