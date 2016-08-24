#include "DxLib.h"

#include "PlayerBullet.hpp"
#include "Keyboard.hpp"
#include <cmath>


PlayerBullet::PlayerBullet()
	: AllBulletNum(4)
	, c_blt(new Counter(10))
	, c_fire(new Counter(2))
	, SPEED(30.0)
	, Angle(DX_PI / 6)
	, LimScXR(-10)
	, LimScXL(650)
	, LimScYT(-10)
	, LimScYB(490)
{
	for (auto &x : Abullet)	x = new Bullet();
	for (auto &x : Bbullet)	x = new Bullet();
	for (auto &x : Cbullet)	x = new Bullet();
	for (auto &x : Dbullet)	x = new Bullet();
	hg_blue		= LoadGraph("GRAPH/GAME/shot0.png");		// Â
	hg_orange	= LoadGraph("GRAPH/GAME/shot1.png");		// ò
//	hg_red		= LoadGraph("");							// Ô Â‚Æ‰©‚ÌŠÔ‚ÌŠp“x‚ğ‚É‚å‚ë‚É‚å‚ë
	hg_eftest	= LoadGraph("GRAPH/GAME/ef_sumple.png");	// ”­ËŒõ

	shiftLevel	= 0;
	isShot		= false;
}


PlayerBullet::~PlayerBullet()
{
	DeleteGraph(hg_eftest);
	DeleteGraph(hg_blue);
	DeleteGraph(hg_orange);
// 	DeleteGraph(hg_red);
	for (auto x : Abullet)	delete x;
	for (auto x : Bbullet)	delete x;
	for (auto x : Cbullet)	delete x;
	for (auto x : Dbullet)	delete x;
}


void PlayerBullet::Update()
{
	const bool PUSH_SHOT_KEY = ( Keyboard_Get(KEY_INPUT_Z) == 1 || Keyboard_Get(KEY_INPUT_RETURN) == 1 );

	// -> Player@‚Ì€–S‚ÉŠÖ‚µ‚Ä‚ÌƒXƒ‹[ˆ—‚ğ‘‚­

	if(PUSH_SHOT_KEY)	Fire(); // z‚Å”­Ë
	
	Move();						// ©‹@’e‚Ì“®‚«
}


void PlayerBullet::Draw()
{
	for (auto &blt : Abullet)
	{
		const bool   isNone = !(blt->f_exist);

		if (isNone)	continue;										// ’e‚ª‘¶İ‚µ‚È‚¢‚È‚ç‚±‚±‚Í–³‹

		const double X_BLT		= blt->x_pos;
		const double Y_BLT		= blt->y_pos;
		const double ROTA_BLT	= -(blt->angle);

		DrawRotaGraph(X_BLT, Y_BLT, 2.0, ROTA_BLT, hg_blue, true);	// ©‹@‚Ì’e‚Ì•`‰æ
	}
}


void PlayerBullet::SetFirePos(const double* x_player, const double* y_player)
{
	for (auto &blt : Abullet)
	{
		const bool isExist = blt->f_exist;

		if (isExist)	continue;					// ©‹@’e‚ª‘¶İ‚·‚é‚È‚ç‚±‚±‚Í–³‹

		blt->x_pos = *x_player - 6.0;
		blt->y_pos = *y_player;
	}
}


// ”­Ë
void PlayerBullet::Fire()
{
	if (shiftLevel == 0)
	{
		for (int i = 0; i < 2; i++)
		{
			const bool	 isExist		= Abullet[i]->f_exist;

			if (isExist)	continue;

			const double X_POS			= Abullet[i]->x_pos;
			const double Y_POS			= Abullet[i]->y_pos - 10;

			FireEffect(X_POS, Y_POS);	// ”­Œõ

			Abullet[i]->f_exist = true;
		}

		return;
	}

	for (int i = 0; i < shiftLevel; i++)
	{
		const bool isExist = Abullet[i]->f_exist;

		if (isExist)	continue;

		const double X_POS = Abullet[i]->x_pos;
		const double Y_POS = Abullet[i]->y_pos - 10;

		// ”­Œõ
		FireEffect(X_POS, Y_POS);

		Abullet[i]->f_exist = true;
	}
}


// ’e‚Ì“®‚«
void PlayerBullet::Move()
{
	if (shiftLevel == 0)
	{
		// ‘¶İ‚µ‚Ä‚¢‚é’e‚ğ“®‚©‚·
		for (int i = 0; i < 2; i++)
		{
			const double X_VECTOR_BLT	= sin(Abullet[i]->angle) * PlayerBullet::SPEED;
			const double Y_VECTOR_BLT	= cos(Abullet[i]->angle) * PlayerBullet::SPEED;
			const bool   isLimitOver	= Abullet[i]->y_pos < LimScYT;
			const bool   isExist		= Abullet[i]->f_exist;

			if (isExist)
			{
				Abullet[i]->x_pos -= X_VECTOR_BLT;
				Abullet[i]->y_pos -= Y_VECTOR_BLT;
				if (isLimitOver)	Reset(i);
			}
		}

/*
		if (Abullet[0]->f_exist)
		{
			Abullet[0]->x_pos -= sin(Abullet[0]->angle) * PlayerBullet::SPEED;
			Abullet[0]->y_pos -= cos(Abullet[0]->angle) * PlayerBullet::SPEED;
			if (Abullet[0]->y_pos < LimScYT)	Reset(0);
		}
*/
		return;
	}

	for (int i = 0; i < shiftLevel; i++)
	{
		// ‘¶İ‚µ‚Ä‚¢‚é’e‚ğ“®‚©‚·
		if (Abullet[i]->f_exist)
		{
			const bool RANGE_OVER = Abullet[i]->y_pos < LimScYT;

			Abullet[i]->x_pos -= sin(Abullet[i]->angle) * PlayerBullet::SPEED;
			Abullet[i]->y_pos -= cos(Abullet[i]->angle) * PlayerBullet::SPEED;
			if (RANGE_OVER)	Reset(i);
			continue;
		}
	}		
}


void PlayerBullet::Reset(int num)
{
	Abullet[num]->f_exist = false;
	Abullet[num]->x_pos   = -100;
	Abullet[num]->y_pos   = -100;
}


void PlayerBullet::SetPlayerPow(int powlv){
	shiftLevel = powlv;
}


void PlayerBullet::FireEffect(double fx, double fy)
{
	DrawRotaGraph(fx - 14.0, fy, 2.0, 0.0, hg_eftest, true);
	DrawRotaGraph(fx + 14.0, fy, 2.0, 0.0, hg_eftest, true);
}