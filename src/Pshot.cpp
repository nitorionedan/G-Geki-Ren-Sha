#include "DxLib.h"

#include "Pshot.hpp"
#include "Keyboard.hpp"
#include "Game.hpp"
#include "DebugMode.hpp"
#include "Player.hpp"
#include "Vector2D.hpp"
#include "Stage.hpp"
#include "EnemyMng.hpp"

#include <algorithm>
#include <cmath>

#define NOMINMAX // 農民マックス！


std::array<Vector2D, 20>	Pshot::s_Apos;
std::array<Vector2D, 30>	Pshot::s_Bpos;
std::array<Vector2D, 30>	Pshot::s_Cpos;


Pshot::Pshot()
	: SCREEN_LIMIT_XL(-10)
	, SCREEN_LIMIT_XR(650)
	, SCREEN_LIMIT_YT(-10)
	, SCREEN_LIMIT_YB(490)

	, A_SPEED(22.0) // 25
	, B_SPEED(18.0) // 20
	, C_SPEED(20.0) // 23
	, BASE_ANGLE(-DX_PI / 2.0)
	, ADD_ANGLE1(0.07) // 0.07
	, ADD_ANGLE2(0.10) // 0.12
	// +8
	, ADD_ANGLE3(0.18) // 0.18
	, ADD_ANGLE4(0.21) // 0.22
	// +8
	, ADD_ANGLE5(0.29) // 0.20
	, ADD_ANGLE6(0.32) // 0.12

	, ANGLE_L_1(BASE_ANGLE - ADD_ANGLE1)
	, ANGLE_L_2(BASE_ANGLE - ADD_ANGLE2)
	, ANGLE_L_3(BASE_ANGLE - ADD_ANGLE3)
	, ANGLE_L_4(BASE_ANGLE - ADD_ANGLE4)
	, ANGLE_L_5(BASE_ANGLE - ADD_ANGLE5)
	, ANGLE_L_6(BASE_ANGLE - ADD_ANGLE6)

	, ANGLE_R_1(BASE_ANGLE + ADD_ANGLE1)
	, ANGLE_R_2(BASE_ANGLE + ADD_ANGLE2)
	, ANGLE_R_3(BASE_ANGLE + ADD_ANGLE3)
	, ANGLE_R_4(BASE_ANGLE + ADD_ANGLE4)
	, ANGLE_R_5(BASE_ANGLE + ADD_ANGLE5)
	, ANGLE_R_6(BASE_ANGLE + ADD_ANGLE6)
	
	, ADD_FIRE_POS(18.)
	, ADD_FIRE_POS_Y(6.)

	, hg_fire(LoadGraph("GRAPH/GAME/pfire.png"))
	, hs_shot(LoadSoundMem("SOUND/SE/pshot00.wav"))
	, hs_hit(LoadSoundMem("SOUND/SE/damage01.wav"))
	, c_shot(new Counter(30))
	, effect(new Effect(new HitEffect))
	, bomb(new Bomb)
	, shiftLevel(0)
	, mPlayer_x(0.0)
	, mPlayer_y(0.0)
	, isShot(false)
	, isShotNext(false)
{
	for (auto &shot : Ashot)	shot = new Bullet();
	for (auto &shot : Bshot)	shot = new Bullet();
	for (auto &shot : Cshot)	shot = new Bullet();
	
	hg.at(eShot_Blue)		= LoadGraph("GRAPH/GAME/shot0.png");
	hg.at(eShot_Orange)		= LoadGraph("GRAPH/GAME/tama.png");
	hg.at(eShot_Red)		= LoadGraph("GRAPH/GAME/shot2.png");
	hg2.at(eShot_Blue)		= LoadGraph("GRAPH/GAME/shot0_2.png");
	hg2.at(eShot_Orange)	= LoadGraph("GRAPH/GAME/tama_2.png");
	hg2.at(eShot_Red)		= LoadGraph("GRAPH/GAME/shot2_2.png");

	ChangeVolumeSoundMem(110, hs_shot);
	ChangeVolumeSoundMem(135, hs_hit);

	for (auto shot : Ashot)	shot->atk = 2;
	for (auto shot : Bshot)	shot->atk = 3;
	for (auto shot : Cshot)	shot->atk = 1;

	x_pos.fill(0.0);
	y_pos.fill(0.0);
	angle.fill(0.0);

	// static------------------------------------
	for (int i = 0; i != s_Apos.size(); ++i)
	{
		s_Apos[i].x = Ashot[i]->x_pos;
		s_Apos[i].y = Ashot[i]->y_pos;
	}

	for (int i = 0; i != s_Bpos.size(); ++i)
	{
		s_Bpos[i].x = Bshot[i]->x_pos;
		s_Bpos[i].y = Bshot[i]->y_pos;
	}

	for (int i = 0; i != s_Cpos.size(); ++i)
	{
		s_Cpos[i].x = Cshot[i]->x_pos;
		s_Cpos[i].y = Cshot[i]->y_pos;
	}

}


Pshot::~Pshot()
{
	for (auto &shot : Ashot)	delete shot;
	for (auto &shot : Bshot)	delete shot;
	for (auto &shot : Cshot)	delete shot;
	for (int &Gr_Handle : hg)	DeleteGraph(Gr_Handle);
	for (int &Gr_Handle : hg2)	DeleteGraph(Gr_Handle);
	DeleteGraph(hg_fire);
	DeleteSoundMem(hs_shot);
	DeleteSoundMem(hs_hit);
}


void Pshot::Update()
{
	shiftLevel = Player::GetShiftLevel();						// シフトレベルを手に入れる

	bomb->Update();
	SetFirePosition(Game::GetPlayerPos().x, Game::GetPlayerPos().y);	// 発射位置を設定
	Input();													// 入力管理
	Move();														// 動き計算
	HitCheck();													// 当たり判定
	effect->Update();											// エフェクト更新
	Reset();													// 弾をリセット
	ShotCount();												// 発射準備のカウント管理
	CopyStaticMem();											// staticメンバに値をコピー
}


void Pshot::Draw()
{
	bomb->Draw();

	for (auto &shot : Ashot) // 青
	{
		if (shot->f_exist == false)	continue;
		
		if(shiftLevel < 3)
			DrawRotaGraph(shot->x_pos, shot->y_pos, 2.0, 0.0, hg[eShot_Blue], true);
		else {
			DrawRotaGraph(shot->x_pos, shot->y_pos, 2.0, 0.0, hg2[eShot_Blue], true);
		}
	}
	// ------------------------------------------------------------------------
	for (auto &shot : Bshot) // 橙
	{
		if (shot->f_exist == false)	continue;
		if (shiftLevel < 3)
			DrawRotaGraph(shot->x_pos, shot->y_pos, 2.0, 0.0, hg[eShot_Orange], true);
		else {
			DrawRotaGraph(shot->x_pos, shot->y_pos, 2.0, 0.0, hg2[eShot_Orange], true);
		}
	}
	// ------------------------------------------------------------------------
	for (auto &shot : Cshot) // 赤
	{
		if (shot->f_exist == false)	continue;
		const double ROTA = ( shot->angle + DX_PI / 2 ); // 回転値を計算
		if(shiftLevel < 3)
			DrawRotaGraph(shot->x_pos, shot->y_pos, 2.0, ROTA, hg[eShot_Red], true);
		else {
			DrawRotaGraph(shot->x_pos, shot->y_pos, 2.0, ROTA, hg2[eShot_Red], true);
		}
	}
	// ------------------------------------------------------------------------
	switch (shiftLevel)
	{
	case 1:
		if (isShot == false)	break;
		if (c_shot->GetNowcount() == 20 || c_shot->GetNowcount() == 15)
		{
			DrawRotaGraph(x_fire[0], y_fire, 2.0, 0.0, hg_fire, true);
			DrawRotaGraph(x_fire[1], y_fire, 2.0, 0.0, hg_fire, true, true);
		}
		break;

	case 2:
		if (isShot == false)	break;
		if (c_shot->GetNowcount() == 30 || c_shot->GetNowcount() == 24 || c_shot->GetNowcount() == 19)
		{
			DrawRotaGraph(x_fire[0], y_fire, 2.0, 0.0, hg_fire, true);
			DrawRotaGraph(x_fire[1], y_fire, 2.0, 0.0, hg_fire, true, true);
		}
		break;

	case 3:
		if (isShot == false)	break;
		if (c_shot->GetNowcount() == 30 || c_shot->GetNowcount() == 24 || c_shot->GetNowcount() == 19)
		{
			DrawRotaGraph(x_fire[0], y_fire, 2.0, 0.0, hg_fire, true);
			DrawRotaGraph(x_fire[1], y_fire, 2.0, 0.0, hg_fire, true, true);
		}
		break;

	case 4:
		if (isShot == false)	break;
		if (c_shot->GetNowcount() == 30 || c_shot->GetNowcount() == 27 || c_shot->GetNowcount() == 24
			|| c_shot->GetNowcount() == 22 || c_shot->GetNowcount() == 19 || c_shot->GetNowcount() == 16)
		{
			DrawRotaGraph(x_fire[0], y_fire, 2.0, 0.0, hg_fire, true);
			DrawRotaGraph(x_fire[1], y_fire, 2.0, 0.0, hg_fire, true, true);
		}
		break;
	}

	// ヒットしたときの効果
	effect->Draw();

	// TEST--------------------------------------------------------------------
	if (DebugMode::isTest == false)	return;
	/*
	for (int i = 0; i != Ashot.size(); ++i)
	{
		if(Ashot[i]->f_exist)
			DrawFormatString(0, 50 + (i * 20), GetColor(0, 255, 0), "Ashot[%d] == true", i);
		if (!Ashot[i]->f_exist)
			DrawFormatString(0, 50 + (i * 20), GetColor(0, 255, 0), "Ashot[%d] == false", i);
	}
	for (int i = 0; i != Cshot.size(); ++i)
	{
		if (Cshot[i]->f_exist)
			DrawFormatString(0, 50 + (i * 20), GetColor(0, 255, 0), "Cshot[%d] == true", i);
		if (!Cshot[i]->f_exist)
			DrawFormatString(0, 50 + (i * 20), GetColor(0, 255, 0), "Cshot[%d] == false", i);
	}
	for (int i = 0; i != Cshot.size(); ++i){
		DrawFormatString(0, 50 + (i * 20), GetColor(0, 255, 0), "C.angle[%d] == %lf", i, Cshot.at(i)->angle);
	}
	DrawFormatString(0, 50, GetColor(0, 255, 0), "count = %d", c_shot->GetNowcount());
	if(isShot)
		DrawFormatString(0, 70, GetColor(0, 255, 0), "isShot     = true");
	else
		DrawFormatString(0, 70, GetColor(0, 255, 0), "isShot     = false");

	if(isShotNext)
		DrawFormatString(0, 90, GetColor(0, 255, 0), "isShotNext = true");
	else
		DrawFormatString(0, 90, GetColor(0, 255, 0), "isShotNext = false");
	*/
}


void Pshot::ShiftReset()
{
	shiftLevel = 0;
}


void Pshot::Input()
{
	const bool PUSH_Z_KEY	= ( Keyboard_Get(KEY_INPUT_Z) == 1 );
	const bool isREADY_NEXT = ( isShotNext && !isShot );				// 次の発射が待っているか？
	const bool isON_NEXT	= ( PUSH_Z_KEY && isShot && !isShotNext );	// 発射中に発射ボタンを押したか？
	const bool isSHOT		= ( PUSH_Z_KEY && !isShot );				// 発射したか？

	if (isREADY_NEXT)
	{
		isShot		= true;				// 発射準備
		isShotNext	= false;			// 次の発射準備をおろす
	}
	if (isON_NEXT)	isShotNext	= true;
	if (isSHOT)		isShot		= true;
	if (isShot)		Fire();				// 発射処理
	if (Keyboard_Get(KEY_INPUT_X) == 1)	bomb->Fire(shiftLevel);
}


void Pshot::ShotCount()
{
	if (c_shot->GetNowcount() == 15)		// カウントが15になると
	{
		c_shot->Reset();					// カウントリセット
		isShot = false;						// 発射をやめる
	}

	if (isShot) {							// 発射中は
		c_shot->Update();					// カウントが作動
	}
}


void Pshot::SetFirePosition(const double X_POS, const double Y_POS)
{
	mPlayer_x = X_POS;
	mPlayer_y = Y_POS;
	// ------------------------------------------------------------------------
	for (auto &shot : Ashot)
	{
		if (shot->f_exist)	continue;	// 存在しているなら次へ
		shot->Set(X_POS, Y_POS);
	}
	// ------------------------------------------------------------------------
	for (auto &shot : Bshot)
	{
		if (shot->f_exist)	continue;
		shot->Set(X_POS, Y_POS);
	}
	// ------------------------------------------------------------------------
	for (auto &shot : Cshot)
	{
		if (shot->f_exist)	continue;
		shot->Set(X_POS, Y_POS);
	}
}


void Pshot::Fire()
{
	// Cshotの発射位置を調整
	const double C_SHOT_XL  = ( mPlayer_x - 18.0 );
	const double C_SHOT_XR	= ( mPlayer_x + 18.0 );
	const double C_SHOT_Y	= ( mPlayer_y + 20.0 );
	for(auto &shot : Cshot)
	{
		if (shot->f_exist)	continue;
		shot->y_pos = C_SHOT_Y;
	}

	// 発火の座標
	x_fire[0] = C_SHOT_XL - 4.;
	x_fire[1] = C_SHOT_XR + 4.;
	y_fire = C_SHOT_Y - 20.;

	// ------------------------------------------------------------------------
	if(shiftLevel == 1)
	{
		const bool isREADY_A = ( c_shot->Remainder(5) == 0 && c_shot->GetNowcount() > 20 );
		/// Bshot なし
		const bool isFIRE_1 = ( c_shot->GetNowcount() == 20 );
		const bool isFIRE_3 = ( c_shot->GetNowcount() == 15 );
		const bool isREADY_C = (isFIRE_1 || isFIRE_3);

		// Ashot ---------------------------------------------------
		if(isREADY_A)
		{
			for(auto &shot : Ashot)
			{
				if (shot->f_exist)	continue;	// 存在しているなら次へ
				shot->f_exist = true;
				PlaySoundMem(hs_shot, DX_PLAYTYPE_BACK);
				break;
			}
		}
		
		/// Bshot なし

		// Cshot ---------------------------------------------------
		if(isREADY_C)
		{
			int count = 0;									// 弾の数合わせ

			for (int i = 0; i != Cshot.size(); ++i)
			{
				if (Cshot.at(i)->f_exist)	continue;		// 存在しているなら次へ

				Cshot.at(i)->f_exist = true;				// 発射準備
				count++;									// 弾の数をカウント

				// 左側
				if (count == 1) {
					Cshot.at(i)->x_pos = C_SHOT_XL;
					if (isFIRE_1)	Cshot.at(i)->angle = ANGLE_L_1;
					if (isFIRE_3)	Cshot.at(i)->angle = ANGLE_L_3;
					PlaySoundMem(hs_shot, DX_PLAYTYPE_BACK);
				}

				// 右側
				if (count == 2) {
					Cshot.at(i)->x_pos = C_SHOT_XR;
					if (isFIRE_1)	Cshot.at(i)->angle = ANGLE_R_1;
					if (isFIRE_3)	Cshot.at(i)->angle = ANGLE_R_3;
					break;									// 弾は２個まで
				}
			}
		}
	}
	// ------------------------------------------------------------------------
	if(shiftLevel == 2)
	{
	const bool isREADY_A  = ( c_shot->Remainder(4) == 2 && c_shot->GetNowcount() > 21 );
		const bool isC_FIRE_1 = (c_shot->GetNowcount() == 30);
		const bool isC_FIRE_3 = (c_shot->GetNowcount() == 24);
		const bool isC_FIRE_5 = (c_shot->GetNowcount() == 19);
		const bool isREADY_C  = ( isC_FIRE_1 || isC_FIRE_3 || isC_FIRE_5 );
		const double A_SHOT_XL = (mPlayer_x - 5.0);
		const double A_SHOT_XR = (mPlayer_x + 5.0);

		// Ashot ---------------------------------------------------
		if (isREADY_A)
		{
			int count = 0;									// 弾の数合わせ

			for (int i = 0; i != Ashot.size(); ++i)
			{
				if (Ashot.at(i)->f_exist)	continue;		// 存在しているなら次へ
				Ashot.at(i)->f_exist = true;				// 発射準備

				count++;									// 弾の数をカウント

				// 左側
				if (count == 1)
				{
					Ashot.at(i)->x_pos = A_SHOT_XL;
					PlaySoundMem(hs_shot, DX_PLAYTYPE_BACK);
				}
				// 右側
				if (count == 2)
				{
					Ashot.at(i)->x_pos = A_SHOT_XR;
					break;									// 弾は２個まで
				}
			}
		}

		/// Bshot なし 
		
		// Cshot ---------------------------------------------------
		if (isREADY_C)
		{
			int count = 0;									// 弾の数合わせ

			for (int i = 0; i != Cshot.size(); ++i)
			{
				if (Cshot.at(i)->f_exist)	continue;		// 存在しているなら次へ
				Cshot.at(i)->f_exist = true;				// 発射準備

				count++;									// 弾の数をカウント

				// 左側
				if (count == 1)
				{
					Cshot.at(i)->x_pos = C_SHOT_XL;
					if (isC_FIRE_1)		Cshot.at(i)->angle = ANGLE_L_1;
					if (isC_FIRE_3)		Cshot.at(i)->angle = ANGLE_L_3;
					if (isC_FIRE_5)		Cshot.at(i)->angle = ANGLE_L_5;
					PlaySoundMem(hs_shot, DX_PLAYTYPE_BACK);
				}
				// 右側
				if (count == 2)
				{
					Cshot.at(i)->x_pos = C_SHOT_XR;
					if (isC_FIRE_1)		Cshot.at(i)->angle = ANGLE_R_1;
					if (isC_FIRE_3)		Cshot.at(i)->angle = ANGLE_R_3;
					if (isC_FIRE_5)		Cshot.at(i)->angle = ANGLE_R_5;
					break;									// 弾は２個まで
				}
			}
		}
	}
	// ------------------------------------------------------------------------
	if(shiftLevel == 3)
	{
		const bool isREADY_A = (c_shot->Remainder(4) == 2);

		const bool isB_FIRE_1 = (c_shot->GetNowcount() == 25); // 25
		const bool isB_FIRE_2 = (c_shot->GetNowcount() == 22); // 20
		const bool isB_FIRE_3 = (c_shot->GetNowcount() == 19); // 15

		const bool isC_FIRE_1 = (c_shot->GetNowcount() == 30);
		const bool isC_FIRE_3 = (c_shot->GetNowcount() == 24);
		const bool isC_FIRE_5 = (c_shot->GetNowcount() == 19);


		const bool isREADY_B =
			(isB_FIRE_1
				|| isB_FIRE_2
				|| isB_FIRE_3 );
		const bool isREADY_C =
			(isC_FIRE_1
				|| isC_FIRE_3
				|| isC_FIRE_5 );
		const double A_SHOT_XL = (mPlayer_x - 5.0);
		const double A_SHOT_XR = (mPlayer_x + 5.0);


		// Ashot ---------------------------------------------------
		if (isREADY_A)
		{
			for(auto &shot : Ashot)
			{
				if (shot->f_exist)	continue;
				shot->f_exist = true;
				break;
			}
		}
		// Bshot ---------------------------------------------------
		if (isREADY_B)
		{
			int count = 0;									// 弾の数合わせ

			for (int i = 0; i != Bshot.size(); ++i)
			{
				if (Bshot.at(i)->f_exist)	continue;		// 存在しているなら次へ

				Bshot.at(i)->f_exist = true;				// 発射準備
				count++;									// 弾の数をカウント

															// 左側１
				if (count == 1) {
					Bshot.at(i)->x_pos = C_SHOT_XL;
					PlaySoundMem(hs_shot, DX_PLAYTYPE_BACK);
				}
				// 左側２
				if (count == 2) {
					Bshot.at(i)->x_pos = C_SHOT_XR;
					break;
				}
			}
		}
		// Cshot ---------------------------------------------------
		if (isREADY_C)
		{
			int count = 0;									// 弾の数合わせ

			for (int i = 0; i != Cshot.size(); ++i)
			{
				if (Cshot.at(i)->f_exist)	continue;		// 存在しているなら次へ
				Cshot.at(i)->f_exist = true;				// 発射準備

				count++;									// 弾の数をカウント

				// 左側
				if (count == 1)
				{
					Cshot.at(i)->x_pos = C_SHOT_XL;
					if (isC_FIRE_1)		Cshot.at(i)->angle = ANGLE_L_1;
					if (isC_FIRE_3)		Cshot.at(i)->angle = ANGLE_L_3;
					if (isC_FIRE_5)		Cshot.at(i)->angle = ANGLE_L_5;
					PlaySoundMem(hs_shot, DX_PLAYTYPE_BACK);
				}
				// 右側
				if (count == 2)
				{
					Cshot.at(i)->x_pos = C_SHOT_XR;
					if (isC_FIRE_1)		Cshot.at(i)->angle = ANGLE_R_1;
					if (isC_FIRE_3)		Cshot.at(i)->angle = ANGLE_R_3;
					if (isC_FIRE_5)		Cshot.at(i)->angle = ANGLE_R_5;
					break;									// 弾は２個まで
				}
			}
		}
	}
	// ------------------------------------------------------------------------
	if(shiftLevel == 4)
	{
		const bool isREADY_A = (c_shot->Remainder(4) == 2);
		
		const bool isB_FIRE_1 = (c_shot->GetNowcount() == 25); // 25
		const bool isB_FIRE_2 = (c_shot->GetNowcount() == 22); // 20
		const bool isB_FIRE_3 = (c_shot->GetNowcount() == 19); // 15

		const bool isC_FIRE_1 = (c_shot->GetNowcount() == 30);
		const bool isC_FIRE_2 = (c_shot->GetNowcount() == 27);
		const bool isC_FIRE_3 = (c_shot->GetNowcount() == 24);
		const bool isC_FIRE_4 = (c_shot->GetNowcount() == 22);
		const bool isC_FIRE_5 = (c_shot->GetNowcount() == 19);
		const bool isC_FIRE_6 = (c_shot->GetNowcount() == 16);

		const bool isREADY_B = (
			isB_FIRE_1
			|| isB_FIRE_2
			|| isB_FIRE_3 );
		const bool isREADY_C = (
			isC_FIRE_1
			|| isC_FIRE_2
			|| isC_FIRE_3
			|| isC_FIRE_4
			|| isC_FIRE_5
			|| isC_FIRE_6 );
		const double A_SHOT_XL = (mPlayer_x - 5.0);
		const double A_SHOT_XR = (mPlayer_x + 5.0);


		// Ashot ---------------------------------------------------
		if (isREADY_A)
		{
			int count = 0;									// 弾の数合わせ

			for (int i = 0; i != Ashot.size(); ++i)
			{
				if (Ashot.at(i)->f_exist)	continue;		// 存在しているなら次へ
				Ashot.at(i)->f_exist = true;				// 発射準備

				count++;									// 弾の数をカウント

				// 左側
				if (count == 1)
				{
					Ashot.at(i)->x_pos = A_SHOT_XL;
					PlaySoundMem(hs_shot, DX_PLAYTYPE_BACK);
				}
				// 右側
				if (count == 2)
				{
					Ashot.at(i)->x_pos = A_SHOT_XR;
					break;									// 弾は２個まで
				}
			}
		}
		// Bshot ---------------------------------------------------
		if (isREADY_B)
		{
			int count = 0;									// 弾の数合わせ

			for (int i = 0; i != Bshot.size(); ++i)
			{
				if (Bshot.at(i)->f_exist)	continue;		// 存在しているなら次へ

				Bshot.at(i)->f_exist = true;				// 発射準備
				count++;									// 弾の数をカウント

				// 左側１
				if (count == 1) {
					Bshot.at(i)->x_pos = C_SHOT_XL;
					Bshot.at(i)->y_pos = C_SHOT_Y;
					PlaySoundMem(hs_shot, DX_PLAYTYPE_BACK);
				}
				// 左側２
				if (count == 2) {
					Bshot.at(i)->x_pos = (C_SHOT_XL - 12.0);
				}
				// 右側１
				if(count == 3) {
					Bshot.at(i)->x_pos = C_SHOT_XR;
					Bshot.at(i)->y_pos = C_SHOT_Y;
				}
				// 右側２
				if(count == 4)
				{
					Bshot.at(i)->x_pos = C_SHOT_XR + 12.0;
					break;
				}
			}
		}
		// Cshot ---------------------------------------------------
		if (isREADY_C)
		{
			int count = 0;									// 弾の数合わせ

			for (int i = 0; i != Cshot.size(); ++i)
			{
				if (Cshot.at(i)->f_exist)	continue;		// 存在しているなら次へ
				Cshot.at(i)->f_exist = true;				// 発射準備

				count++;									// 弾の数をカウント

				// 左側
				if (count == 1)
				{
					Cshot.at(i)->x_pos = C_SHOT_XL;
					if (isC_FIRE_1)		Cshot.at(i)->angle = ANGLE_L_1;
					if (isC_FIRE_2)		Cshot.at(i)->angle = ANGLE_L_2;
					if (isC_FIRE_3)		Cshot.at(i)->angle = ANGLE_L_3;
					if (isC_FIRE_4)		Cshot.at(i)->angle = ANGLE_L_4;
					if (isC_FIRE_5)		Cshot.at(i)->angle = ANGLE_L_5;
					if (isC_FIRE_6)		Cshot.at(i)->angle = ANGLE_L_6;
					PlaySoundMem(hs_shot, DX_PLAYTYPE_BACK);
				}
				// 右側
				if (count == 2)
				{
					Cshot.at(i)->x_pos = C_SHOT_XR;
					if (isC_FIRE_1)		Cshot.at(i)->angle = ANGLE_R_1;
					if (isC_FIRE_2)		Cshot.at(i)->angle = ANGLE_R_2;
					if (isC_FIRE_3)		Cshot.at(i)->angle = ANGLE_R_3;
					if (isC_FIRE_4)		Cshot.at(i)->angle = ANGLE_R_4;
					if (isC_FIRE_5)		Cshot.at(i)->angle = ANGLE_R_5;
					if (isC_FIRE_6)		Cshot.at(i)->angle = ANGLE_R_6;
					break;									// 弾は２個まで
				}
			}
		}
	}
	// ------------------------------------------------------------------------

}


void Pshot::Move()
{
	// Ashot --------------------------------------
	for (auto &shot : Ashot)
	{
		if (shot->f_exist == false)	continue;	// 存在していないなら次へ

		shot->y_pos -= A_SPEED;
	}

	// Bshot --------------------------------------
	for(auto &shot : Bshot)
	{
		if (shot->f_exist == false)	continue;

		shot->x_pos += std::sin((double)Stage::GetTime()) * 4.0;	// 軌道をぐにゃらせる
		shot->y_pos -= B_SPEED;
	}

	// Cshot --------------------------------------
	for (auto &shot : Cshot)
	{
		if (shot->f_exist == false)	continue;

		const double X_VECTOR = std::cos(shot->angle) * C_SPEED;
		const double Y_VECTOR = std::sin(shot->angle) * C_SPEED;

		shot->x_pos += X_VECTOR;
		shot->y_pos += Y_VECTOR;
	}
}


void Pshot::HitCheck()
{
	for(auto &shot : Ashot)
	{
		if (!shot->f_exist)	continue;
		const bool IS_HIT = ( Game::IsHitBoss(shot->x_pos, shot->y_pos, shot->atk) ||
								EnemyMng::IsHit(shot->x_pos, shot->y_pos, shot->atk) );
		if (IS_HIT)	shot->f_exist = false;
		if (IS_HIT)	PlaySoundMem(hs_hit, DX_PLAYTYPE_BACK);
		if (IS_HIT)	effect->PlayAnime(shot->x_pos, shot->y_pos);
	}
	// -------------------------------------------------------------------------------
	for (auto &shot : Bshot)
	{
		if (!shot->f_exist)	continue;
		const bool IS_HIT = (Game::IsHitBoss(shot->x_pos, shot->y_pos, shot->atk) ||
								EnemyMng::IsHit(shot->x_pos, shot->y_pos, shot->atk));
		if (IS_HIT)	shot->f_exist = false;
		if (IS_HIT)	PlaySoundMem(hs_hit, DX_PLAYTYPE_BACK);
		if (IS_HIT)	effect->PlayAnime(shot->x_pos, shot->y_pos);
	}
	// -------------------------------------------------------------------------------
	for (auto &shot : Cshot)
	{
		if (!shot->f_exist)	continue;
		const bool IS_HIT = (Game::IsHitBoss(shot->x_pos, shot->y_pos, shot->atk) ||
								EnemyMng::IsHit(shot->x_pos, shot->y_pos, shot->atk));
		if (IS_HIT)	shot->f_exist = false;
		if (IS_HIT)	PlaySoundMem(hs_hit, DX_PLAYTYPE_BACK);
		if (IS_HIT)	effect->PlayAnime(shot->x_pos, shot->y_pos);
	}
}


void Pshot::Reset()
{
	for(auto &shot : Ashot)
	{
		// 画面外処理
		if (shot->f_exist == false)	continue;	// 存在していないなら次へ
		if (shot->x_pos < SCREEN_LIMIT_XL || SCREEN_LIMIT_XR < shot->x_pos)
		{
			shot->f_exist = false;
			continue;
		}
		if (shot->y_pos < SCREEN_LIMIT_YT || SCREEN_LIMIT_YB < shot->y_pos)
			shot->f_exist = false;
	}

	// ------------------------------------------------------------------------
	for (auto &shot : Bshot)
	{
		// 画面外処理
		if (shot->f_exist == false)	continue;
		if (shot->x_pos < SCREEN_LIMIT_XL || SCREEN_LIMIT_XR < shot->x_pos)
		{
			shot->f_exist = false;
			continue;
		}
		if (shot->y_pos < SCREEN_LIMIT_YT || SCREEN_LIMIT_YB < shot->y_pos)
			shot->f_exist = false;
	}

	// ------------------------------------------------------------------------
	for (auto &shot : Cshot)
	{
		// 画面外処理
		if (shot->f_exist == false)	continue;
		if (shot->x_pos < SCREEN_LIMIT_XL || SCREEN_LIMIT_XR < shot->x_pos)
		{
			shot->f_exist = false;
			continue;
		}
		if (shot->y_pos < SCREEN_LIMIT_YT || SCREEN_LIMIT_YB < shot->y_pos)
			shot->f_exist = false;
	}
}


void Pshot::SetAtk(const int Aatk, const int Batk, const int Catk)
{
	for (auto shot : Ashot)	shot->atk = Aatk;
	for (auto shot : Bshot)	shot->atk = Batk;
	for (auto shot : Cshot)	shot->atk = Catk;
}


void Pshot::CopyStaticMem()
{
	for (int i = 0; i != Ashot.size(); ++i)
	{
		if (!Ashot[i]->f_exist)	continue;
		s_Apos[i].x = Ashot[i]->x_pos;
		s_Apos[i].y = Ashot[i]->y_pos;
	}

	for (int i = 0; i != Bshot.size(); ++i)
	{
		if (!Bshot[i]->f_exist)	continue;
		s_Bpos[i].x = Bshot[i]->x_pos;
		s_Bpos[i].y = Bshot[i]->y_pos;
	}

	for (int i = 0; i != Cshot.size(); ++i)
	{
		if (!Cshot[i]->f_exist)	continue;
		s_Cpos[i].x = Cshot[i]->x_pos;
		s_Cpos[i].y = Cshot[i]->y_pos;
	}

}
// EOF