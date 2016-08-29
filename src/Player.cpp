#pragma warning(disable:4244)

#include "DxLib.h"

#include "Player.hpp"
#include "Vector2D.hpp"
#include "DebugMode.hpp"
#include "Stage.hpp"
#include "Game.hpp"
#include "Graphics2D.hpp"

#include <algorithm>
#include <cassert>

#define NOMINMAX


const int Player::MaxPowLevel = 4;
const int Player::MAX_RENSHA = 40;
const int Player::MIN_RENSHA = 0;
const int Player::HIT_RANGE = 8;

ePlayerState Player::state;
Vector2D Player::s_pos;
Vector2D Player::vec;
int Player::s_powlv;
int Player::s_bombNum;
int Player::s_life;
int Player::s_rensha;
int Player::hs_shiftUp;
int Player::hs_shiftDown;
int Player::hs_dead;
int Player::bombNum;
bool Player::s_isDead;
bool Player::s_isMuteki;
bool Player::s_isStart;
bool Player::isHit;
bool Player::isArm;


Player::Player()
	: SpeedForce(7.0)
	, Y_START(360.0)
	, LimitScreenXL(10.0)
	, LimitScreenXR(630.0)
	, LimitScreenYT(10.0)
	, LimitScreenYB(455.0)
	, graphic(new Graphic)
	, c_start(new Counter(150))
	, c_dead(new Counter(90))
	, elapsedTime(0)
	, keydir(InputDir::Neutral)
	, dead_ef(eSpread_SmallGrey)
	, isDead(false)
	, life(3)
	, isMuteki(false)
{
	LoadDivGraph("GRAPH/GAME/player.png", 3, 3, 1, 20, 26, hg);	// 画像の分割読み込み
	LoadDivGraph("GRAPH/GAME/p_shield.png", 12, 6, 2, 26, 39, hg_arm);
	hs_shiftUp = LoadSoundMem("SOUND/SE/shiftup.mp3");
	hs_shiftDown = LoadSoundMem("SOUND/SE/shiftdown.wav");
	hs_dead = LoadSoundMem("SOUND/SE/explosion02.wav");

	pos.SetVecor2D(320.0, 520.0);
	bombNum = 3;
	s_powlv		= 0;
	s_isStart	= false;
	isHit = false;
	s_isMuteki = true;
	isArm = false;
	state = ePlayerState_Start;
}


Player::~Player()
{
	for (int i = 0; i < sizeof(hg) / sizeof(hg[0]); i++)	DeleteGraph(hg[i]);
	for (int i = 0; i < sizeof(hg_arm) / sizeof(hg_arm[0]); i++)	DeleteGraph(hg_arm[i]);
	DeleteSoundMem(hs_shiftUp);
	DeleteSoundMem(hs_shiftDown);
	DeleteSoundMem(hs_dead);
}


void Player::Update()
{
	elapsedTime++;

	switch (state)
	{
	case ePlayerState_Start: Update_Start();	break;
	case ePlayerState_Game: Update_Game();		break;
	case ePlayerState_Dead: Update_Dead();		break;
	default:	printfDx("Player.cpp:ERROR\n");	break;
	}

	// staticメンバ変数に値をコピー（アホ）
	CopyStaticMem();

	isHit = false;

	// TEST------------------------------------------------------------------------------------
	if (DebugMode::isTest == false)	return;
}


void Player::Draw()
{
	// アーム
	if (isArm)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawAnime(pos.x, pos.y + 4., 0., sizeof(hg_arm) / sizeof(hg_arm[0]), 4, hg_arm);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	// 自機
	switch (state)
	{
	case ePlayerState_Start: Draw_Start();	break;
	case ePlayerState_Game: Draw_Game();	break;
	case ePlayerState_Dead: Draw_Dead();	break;
	default:	printfDx("Player.cpp:ERROR\n");	break;
	}

	// TEST------------------------------------------------------------------------------------
	if (DebugMode::isTest == false)	return;

	DrawCircle(pos.x, pos.y + 9, HIT_RANGE, GetColor(0, 255, 0), false);
//	DrawFormatString(100, 100, GetColor(0, 255, 0), "rensha = %d", s_rensha);
//	DrawFormatString(100, 100, GetColor(0, 255, 0), "Dで死にます");
//	DrawFormatString(100, 120, GetColor(0, 255, 0), "Pでシフトアップ");
//	DrawFormatString(100, 140, GetColor(0, 255, 0), "Oでシフトダウン");
}


void Player::Update_Start()
{
	const bool isStartPosition = (pos.y == Y_START);

	c_start->Update();

	if (c_start->isEach(70, 149))
	{
		pos.y -= 2.0;						// 上に上昇
		pos.y = std::max(pos.y, Y_START);	// スタート地点まで
	}else{
		InputMng();
		Rensha_Update();
		Move();
	}

	if (c_start->isLast())
	{
		s_isMuteki = false;
		state = ePlayerState_Game;	// スタート地点ならスタートする
	}
}


void Player::Update_Game()
{
	InputMng();
	Rensha_Update();
	Move();
}


void Player::Update_Dead()
{
	c_dead->Update();

	const bool& IS_DIEING = (c_dead->Remainder(8) == 0 && c_dead->isEach(1, 40));

	if (IS_DIEING)
	{		
		if (c_dead->GetNowcount() == 40)
			Game::PlayAnime(pos.x + 19., pos.y - 23., eExplosion_small);
		if (c_dead->GetNowcount() == 32)
			Game::PlayAnime(pos.x - 3., pos.y + 68., eExplosion_small);
		if (c_dead->GetNowcount() == 24)
			Game::PlayAnime(pos.x - 12., pos.y - 3., eExplosion_small);
		if (c_dead->GetNowcount() == 16)
			Game::PlayAnime(pos.x + 52., pos.y - 56., eExplosion_small);
		if (c_dead->GetNowcount() == 8)
			Game::PlayAnime(pos.x - 39., pos.y + 27., eExplosion_small);
	}

	pos.x += vec.x;
	pos.y += vec.y;

	if(c_dead->isLast())
	{
		life--;
		if (life == 0) {
			/// 死亡
			Game::GameOver();
		}

		// スタート状態にする
		state = ePlayerState_Start;

		// スタート地点に戻す
		pos.SetVecor2D(320.0, 520.0);

		keydir = InputDir::Neutral;

		// 再充填
		bombNum = 3;
	}
}


void Player::Draw_Start()
{
	const bool& IS_STARTING = (c_start->Remainder(4) >= 3);

	if (IS_STARTING)
	{
		SetDrawBlendMode(DX_BLENDMODE_INVSRC, 200); //反転合成
		
		//Draw関数
		switch (keydir)
		{
		case InputDir::Left:	DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[0], true);	break;
		case InputDir::Neutral:	DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[1], true);	break;
		case InputDir::Right:	DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[2], true);	break;
		}
		
		SetDrawBlendMode(DX_BLENDMODE_ADD, 200); //加算合成
												 
		//Draw関数で上と同じ物を重ねる
		switch (keydir)
		{
		case InputDir::Left:	DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[0], true);	break;
		case InputDir::Neutral:	DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[1], true);	break;
		case InputDir::Right:	DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[2], true);	break;
		}
		
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	else
	{
		switch (keydir)
		{
		case InputDir::Left:	DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[0], true);	break;
		case InputDir::Neutral:	DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[1], true);	break;
		case InputDir::Right:	DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[2], true);	break;
		}
	}
}


void Player::Draw_Game()
{
	switch (keydir)
	{
	case InputDir::Left:	DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[0], true);	break;
	case InputDir::Neutral:	DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[1], true);	break;
	case InputDir::Right:	DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[2], true);	break;
	}

	/// ←ここにシールドアニメ
}


void Player::Draw_Dead()
{
	if (elapsedTime % 4 >= 2)						// 2fps毎に元の色に戻す
	{
		//SetDrawBlendMode(DX_BLENDMODE_INVSRC, 100);	// On-Damage-Effect
		//SetDrawBright(255, 0, 0);
		//SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);

		SetDrawBright(255, 0, 0);	// 赤色に設定
	}

	switch (keydir)
	{
	case InputDir::Left:	DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[0], true);	break;
	case InputDir::Neutral:	DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[1], true);	break;
	case InputDir::Right:	DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[2], true);	break;
	}

	SetDrawBright(255, 255, 255);	// 元の色合い
}


void Player::SetStart()
{
	if (s_isStart)	return;

	const bool isStartPosition = (pos.y == Y_START);

	c_start->Update();
	pos.y -= 2.0;								// 上に上昇
	pos.y = std::max(pos.y, Y_START);			// スタート地点まで

	if (isStartPosition)	s_isStart = true;	// スタート地点ならスタートする
}


void Player::InputMng()
{
	const bool KEY_STAY_RIGHT = ( Keyboard_Get(KEY_INPUT_RIGHT) >= 30 );
	const bool KEY_STAY_LEFT  = ( Keyboard_Get(KEY_INPUT_LEFT) >= 30 );
	const bool PUSH_Z_KEY     = ( Keyboard_Get(KEY_INPUT_Z) == 1 );

	keydir = InputDir::Neutral;						// キーをニュートラルにする
	
	if (KEY_STAY_RIGHT)	keydir = InputDir::Right;

	if (KEY_STAY_LEFT)	keydir = InputDir::Left;

	if (PUSH_Z_KEY && s_powlv == 0)	Shift(true);	// 無の状態で押すと無条件で１段階アップ
}


void Player::Move()
{
	const bool INPUT_HORIZONTAL  = Keyboard_Get(KEY_INPUT_RIGHT) >= 1 || Keyboard_Get(KEY_INPUT_LEFT) >= 1;
	const bool INPUT_VERTICAL    = Keyboard_Get(KEY_INPUT_UP)    >= 1 || Keyboard_Get(KEY_INPUT_DOWN) >= 1;
	const bool KEY_STAY_RIGHT    = Keyboard_Get(KEY_INPUT_RIGHT) >= 1;
	const bool KEY_STAY_LEFT     = Keyboard_Get(KEY_INPUT_LEFT)  >= 1;
	const bool KEY_STAY_UP       = Keyboard_Get(KEY_INPUT_UP)    >= 1;
	const bool KEY_STAY_DOWN     = Keyboard_Get(KEY_INPUT_DOWN)  >= 1;
	double speed;


	// スピード設定
	if(INPUT_HORIZONTAL)
	{
		if (INPUT_VERTICAL)	speed = (1 / 1.41421356) * SpeedForce;	// (1 / √2) * speed    // 斜めのとき
		else				speed = SpeedForce;						// 斜め補正解除
	}
	else if(INPUT_VERTICAL)	speed = SpeedForce;						// 上下のみの移動

	// 移動させる
	if (KEY_STAY_RIGHT)	pos.x += speed;
	if (KEY_STAY_LEFT)	pos.x -= speed;
	if (KEY_STAY_UP)	pos.y -= speed;
	if (KEY_STAY_DOWN)	pos.y += speed;

	// 移動可能範囲を設定
	Vector2D::BorderStop(&pos.x, LimitScreenXL, LimitScreenXR);
	Vector2D::BorderStop(&pos.y, LimitScreenYT, LimitScreenYB);
}


void Player::CopyStaticMem()
{
	s_pos		= pos;
	s_bombNum	= bombNum;
	s_life		= life;
	s_isDead	= isDead;
	s_isMuteki	= isMuteki;
}


void Player::Rensha_Update()
{
	// 連射ゲージ加算-----------------------------------------------------
	if(Keyboard_Get(KEY_INPUT_Z) == 1 || Keyboard_Get(KEY_INPUT_A) == 1){
		if(s_rensha < 41)	s_rensha++;
	}

	// -------------------------------------------------------------------
	switch (s_powlv)
	{
	case 0:	s_rensha = 0;	break;
	case 1:	if (Stage::GetTime() % 17 == 0)	s_rensha--;	break;
	case 2:	if (Stage::GetTime() % 12 == 0)	s_rensha--;	break;
	case 3: if (Stage::GetTime() % 11 == 0)	s_rensha--;	break;
	case 4: if (Stage::GetTime() % 10 == 0)	s_rensha--;	break;

	default: assert(!"不正な状態"); break;
	}

	// -------------------------------------------------------------------
	switch (s_rensha)
	{
	case 0:		
		if(s_powlv != 0) Shift(false);
		break;
	case 40:	if (s_powlv != 4)	Shift(true);
		break;
	}
}


void Player::SetArm()
{
	if(isArm)
	{
		Score::AddScore(10000);
		return;
	}

	isArm = true;
}


void Player::Shift(const bool isUP)
{
	if (isUP) {
		s_powlv++;											// 弾レベルを１段階パワーアップ
		if (s_powlv != 1)
		{
			s_rensha = 1 + (2 * s_powlv);	// シフトアップボーナス４メモリ
			PlaySoundMem(hs_shiftUp, DX_PLAYTYPE_BACK);
		}
	}
	else {
		s_powlv--;
		s_rensha = MAX_RENSHA - 4;
		if (s_powlv == 0)	s_rensha = 0;
		if(s_powlv != 0)	PlaySoundMem(hs_shiftDown, DX_PLAYTYPE_BACK);
	}

	s_powlv = std::min(MaxPowLevel, std::max(0, s_powlv));	// レベルを基底の範囲内にする

//	effector->Shift(isUP, s_powlv);							// シフトレベルに応じた演出を開始
}

void Player::AddBomb(){
	bombNum++;
}


void Player::ShiftReset()
{
	s_powlv = 0;
}

void Player::DownBombNum()
{
	if (bombNum == 0)	return;
	bombNum--;
}


void Player::GetPos(double* x, double* y)
{
	*x = this->pos.x;
	*y = this->pos.y;
}


Vector2D& Player::GetPos(){
	return pos;
}

// Static Methods ---------------------------------------------------------------------------------


int Player::GetShiftLevel(){
	return s_powlv;
}


int Player::GetBombNum(){
	return s_bombNum;
}


int Player::GetLife(){
	return s_life;
}


int Player::GetRensha(){
	return s_rensha;
}


bool Player::IsDead(){
	return s_isDead;
}


bool Player::IsMuteki(){
	return s_isMuteki;
}


bool Player::IsStart(){
	return s_isStart;
}


bool Player::HitCheckCircle(const double& ColX, const double& ColY)
{
	if (state == ePlayerState_Dead)	return false;

	const bool& IS_HIT = (Vector2D::CirclePointCollision(s_pos.x, s_pos.y + 9.0, ColX, ColY, HIT_RANGE));

	if(IS_HIT && state == ePlayerState_Game)
	{
		s_powlv = 0;
		state = ePlayerState_Dead;
		isHit = true;
		s_isMuteki = true;
		vec.SetVecor2D(std::cos(1.5 * GetRand(100)), 1.5 * std::cos(GetRand(100)));
		Game::PlaySpread(s_pos.x, s_pos.y, GetRand(100), dead_ef);
		PlaySoundMem(hs_dead, DX_PLAYTYPE_BACK);
	}

	return IS_HIT;
}


bool Player::HitCheckCircle(const double & Range1, const double & Range2, const double & X1, const double & Y1, const double & X2, const double & Y2)
{
	if (state == ePlayerState_Dead)	return false;

	const bool& IS_HIT = (Vector2D::CirclesCollision(Range1, Range2, X1, Y1, X2, Y2));

	// アームを剥がす
	if (IS_HIT)	isArm = false;

	if(IS_HIT && state == ePlayerState_Game)
	{
		s_powlv = 0;
		state = ePlayerState_Dead;
		isHit = true;
		s_isMuteki = true;
		vec.SetVecor2D(std::cos(1.5 * GetRand(100)), 1.5 * std::cos(GetRand(100)));
		Game::PlaySpread(s_pos.x, s_pos.y, GetRand(100), dead_ef);
		Game::PlayQuake();
		PlaySoundMem(hs_dead, DX_PLAYTYPE_BACK);
	}

	return IS_HIT;
}
