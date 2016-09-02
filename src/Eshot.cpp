#include "DxLib.h"

#include "Eshot.hpp"
#include "BossA.hpp"
#include "DebugMode.hpp"
#include "Game.hpp"
#include "Bomb.hpp"
#include "HitEffect.hpp"

#include <cmath>


const double Eshot::SC_LIMIT_XL = -10.;
const double Eshot::SC_LIMIT_XR = 650.;
const double Eshot::SC_LIMIT_YT = -10.;
const double Eshot::SC_LIMIT_YB = 490.;
const double Eshot::HIT_RANGE = 4.;

bool Eshot::s_isFire1;
bool Eshot::s_isFire2;


Eshot::Eshot()
	: A_INTERVAL(4)
	, A_FIRE_NUM(32)
	, A_SPEED(8.) // 8.0
	, A_BASE_ANGLE(DX_PI / 2.)
	, effect(new Effect(new HitEffect))
	, c_atk1(new Counter(240))
	, c_atk2(new Counter(60))
	, c_fire1(new Counter(A_INTERVAL * A_FIRE_NUM))
	, c_fire2(new Counter(100))
	, atkState(eAttackState::Atk1)
	, loopCount(0)
	, speed(0.0)
{
	for (auto &shot : shotA)	shot = new Bullet;

	hg_shotA = LoadGraph("GRAPH/GAME/Eshot/eshot01.png");
	hg_fireA = LoadGraph("GRAPH/GAME/Eshot/efire0.png");
	hs_shotA = LoadSoundMem("SOUND/SE/eshot01.wav");

	x_fireA.fill(0.);
	y_fireA.fill(0.);
	s_isFire1 = false;
}


Eshot::~Eshot()
{
	for (auto &shot : shotA)	delete shot;
	DeleteGraph(hg_shotA);
	DeleteGraph(hg_fireA);
	DeleteSoundMem(hs_shotA);
}


void Eshot::Update(const Player& player)
{	
	SetFirePos();							// 発射位置を設定
	Fire();									// 発射処理
	Move(player);							// 弾の動きを計算

	if (s_isFire1)	c_fire1->Update();		// 攻撃開始からカウントダウン
	if (s_isFire2)	c_fire2->Update();

	effect->Update();
}


void Eshot::Draw()
{
	// 発射光
	if (c_fire1->Remainder(A_INTERVAL) == 0 &&
		s_isFire1 &&
		!BossA::IsDead())
	{
		DrawRotaGraph(x_fireA[0], y_fireA[0], 2.0, 0.0, hg_fireA, true);
		DrawRotaGraph(x_fireA[1], y_fireA[1], 2.0, 0.0, hg_fireA, true);
	}

	for (auto &shot : shotA)
	{
		if (shot->f_exist == false)	continue;			// 存在していないなら次へ

		const double ROTA = (shot->angle + DX_PI / 2);	// 向きを計算

		DrawRotaGraph(shot->x_pos, shot->y_pos, 2.0, ROTA, hg_shotA, true);

		// 当たる範囲を可視化
		if (DebugMode::isTest)
			DrawCircle(shot->x_pos, shot->y_pos, 4, GetColor(0, 255, 0), false);
	}

	effect->Draw();

	// TEST
	if (DebugMode::isTest == false)	return;

	/*
	for (int i = 0; i != shotA.size(); ++i)
	{
		if (shotA[i]->f_exist) {
			DrawFormatString(100, 40 + (i * 20), GetColor(0, 255, 0), "shotA[%d].f_exist = true", i);
		}
		else {
			DrawFormatString(0, 40 + (i * 20), GetColor(0, 255, 0), "shotA[%d].f_exist = false", i);
		}
	}

	DrawFormatString(100, 80, GetColor(0, 255, 0), "shotA[0]->x = %lf", shotA[0]->x_pos);
	
	DrawFormatString(100, 40, GetColor(0, 255, 0), "c_atk1  = %d", c_atk1->GetNowcount());
	DrawFormatString(100, 60, GetColor(0, 255, 0), "c_fire1 = %d", c_fire1->GetNowcount());
	*/
}


void Eshot::SetFirePos()
{
	if (BossA::IsDead())	return; // 死んでいるならスルー

	for(auto &shot : shotA)
	{
		if (shot->f_exist)	continue;
		shot->x_pos = BossA::GetPos().x;
		shot->y_pos = BossA::GetPos().y;
	}
}


void Eshot::Fire()
{
	// 死んでいるならここで返す
	if (BossA::IsDead())	return;						
	
	// 発射準備が整っていないならここで返す
	if (s_isFire1 == false)	return;						


	const bool IS_FIRE_FINISH = (loopCount > A_FIRE_NUM);


	// --------------------------------------------------------------------------------------------------------
	if (atkState == eAttackState::Atk1)
	{
		// ショットAを発射するタイミングが来てないならここで返す
		if (c_fire1->Remainder(A_INTERVAL) != 0)	return;

		int shotCount = 0;										// 二対発射のためのカウンター

		for (int i = 0; i != shotA.size(); ++i)
		{
			if (shotA[i]->f_exist)	continue;					// 存在しているなら次へ

			if (IS_FIRE_FINISH)
			{
				s_isFire1 = false;								// 攻撃終了
				loopCount = 0;									// ループカウントをリセット
				break;
			}

			// 撃った弾の数を数える
			shotCount++;

			switch (shotCount)
			{
			case 1:
				shotA[i]->f_exist = true;
				shotA[i]->angle = loopCount * (-A_BASE_ANGLE / 4.0);			// 約22.5°ずつ
				shotA[i]->x_pos = BossA::GetPos().x + std::cos(shotA[i]->angle) * 60.;
				shotA[i]->y_pos = BossA::GetPos().y + std::sin(shotA[i]->angle) * 60.;
				x_fireA[0] = shotA[i]->x_pos;
				y_fireA[0] = shotA[i]->y_pos;
				PlaySoundMem(hs_shotA, DX_PLAYTYPE_BACK);
				break;

			case 2:
				shotA[i]->f_exist = true;
				shotA[i]->angle = DX_PI + loopCount * (-A_BASE_ANGLE / 4.0);	// 反対側から約22.5°ずつ
				shotA[i]->x_pos = BossA::GetPos().x + std::cos(shotA[i]->angle) * 60.;
				shotA[i]->y_pos = BossA::GetPos().y + std::sin(shotA[i]->angle) * 60.;
				x_fireA[1] = shotA[i]->x_pos;
				y_fireA[1] = shotA[i]->y_pos;

				loopCount++;
				break;	// 弾を同時に２つ発射したら抜ける
			}
		}		
	}
	// --------------------------------------------------------------------------------------------------------
	if (atkState == eAttackState::Atk2)
	{
	}
}


void Eshot::Move(const Player& player)
{
	for(auto &shot : shotA)
	{
		// 存在していないなら次へ
		if (shot->f_exist == false)	continue;

		shot->x_pos += std::cos(shot->angle) * A_SPEED;
		shot->y_pos += std::sin(shot->angle) * A_SPEED;

		const bool& IS_HIT = Vector2D::CirclesCollision(HIT_RANGE, Player::HIT_RANGE,
							shot->x_pos, shot->y_pos, player.GetPos().x, player.GetPos().y);
		const bool& IS_HIT2 = Bomb::IsHit(4, shot->x_pos, shot->y_pos);
		const bool& IS_OUT = isOverLimit(shot->x_pos, shot->y_pos);

		if (IS_HIT || IS_HIT2)
		{
			shot->f_exist = false;
			effect->PlayAnime(shot->x_pos, shot->y_pos);
		}

		// 画面外なら除外リセット
		if(IS_OUT)	shot->f_exist = false;	
	}
}


bool Eshot::isOverLimit(const double X_POS, const double Y_POS)
{
	if (X_POS < SC_LIMIT_XL || SC_LIMIT_XR < X_POS || Y_POS < SC_LIMIT_YT || SC_LIMIT_YB < Y_POS) {
		return true;
	}
	return false;
}


bool Eshot::IsHit(const double & otherRange, const Vector2D other)
{
	return false;
}