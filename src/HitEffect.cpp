#include "DxLib.h"

#include "HitEffect.hpp"


HitEffect::HitEffect()
{
	for (auto &c : c_hit)
		c = new Counter(16);
	for (int i = 0; i != pos.size(); ++i)
		pos[i] = Vector2D::ZERO;
	for (bool &b : isHit)
		b = false;
	LoadDivGraph("GRAPH/GAME/EFFECT/hit128x32.png", 4, 4, 1, 32, 32, gh_hit);

	angle.fill(0.0);
	exrate.fill(2.0);
}


HitEffect::~HitEffect()
{
	for (auto c : c_hit) delete c;
	for (int i = 0; i < _countof(gh_hit); i++)
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
		if (isHit[i] == false)
			continue;

		int count = c_hit[i]->GetNowcount();

		if (c_hit[i]->isEach(13, 15))
			DrawRotaGraph(pos[i].x, pos[i].y, exrate[i], angle[i], gh_hit[0], true);
		if (c_hit[i]->isEach(9, 12))
			DrawRotaGraph(pos[i].x, pos[i].y, exrate[i], angle[i], gh_hit[1], true);
		if (c_hit[i]->isEach(5, 8))
			DrawRotaGraph(pos[i].x, pos[i].y, exrate[i], angle[i], gh_hit[2], true);
		if (c_hit[i]->isEach(1, 4))
			DrawRotaGraph(pos[i].x, pos[i].y, exrate[i], angle[i], gh_hit[3], true);
	}
}


void HitEffect::PlayAnime(const double & MyX, const double & MyY)
{
	for (int i = 0; i != isHit.size(); ++i)
	{
		if (isHit[i] == true)	continue;
		isHit[i] = true;
		pos[i].SetVec(MyX, MyY);
		angle[i] = GetRand(10) / 10.0;
//		exrate[i] = (GetRand(10) / 10.0) + 1.0;
		break;
	}
}


// ===============================================================-
std::shared_ptr<HitEffect> IHitEffect::mHitEffect;


void IHitEffect::set(std::shared_ptr<HitEffect> hitEffect)
{
	mHitEffect = hitEffect;
}
