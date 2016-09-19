#include "Player.hpp"
#include "DebugMode.hpp"
#include "IScore.hpp"
#include "Graphics2D.hpp"
#include "Effector.hpp"
#include "Keyboard.hpp"
#include "Graphic.hpp"
#include "Counter.hpp"
#include "PieceEffect.hpp"
#include "ExplosionEffect.hpp"
#include "Bomb.hpp"
#include "Pshot.hpp"
#include "Stage.hpp"
#include "Game.hpp"

#include <DxLib.h>
#include <algorithm>
#include <cassert>

#undef max
#undef min


const int Player::MaxPowLevel = 4;
const int Player::MAX_RENSHA = 40;
const int Player::MIN_RENSHA = 0;
const int Player::HIT_RANGE = 8;


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
	, keydir(eInputDir::Neutral)
	, dead_ef(eSpread_SmallGrey)
	, life(3)
{
	LoadDivGraph("GRAPH/GAME/player.png", 3, 3, 1, 20, 26, hg);	// 画像の分割読み込み
	LoadDivGraph("GRAPH/GAME/p_shield.png", 12, 6, 2, 26, 39, hg_arm);
	hs_shiftUp = LoadSoundMem("SOUND/SE/shiftup.mp3");
	hs_shiftDown = LoadSoundMem("SOUND/SE/shiftdown.wav");
	hs_dead = LoadSoundMem("SOUND/SE/explosion02.wav");
	Screen = MakeScreen(640, 480, TRUE);

	pos.SetVec(320.0, 520.0);
	vec = Vector2D::ZERO;
	bombNum = 3;
	powlv = 0;
	isDead = false;
	isStart = false;
	isHit = false;
#ifdef _DEBUG
	isMuteki = true;
	printfDx("無敵だよ！\n");
#else
	isMuteki = false;
	printfDx("無敵じゃないよ！\n");
#endif
	isArm = false;
	state = ePlayerState::Start;
}


Player::~Player()
{
	for (int i = 0; i < sizeof(hg) / sizeof(hg[0]); i++)	DeleteGraph(hg[i]);
	for (int i = 0; i < sizeof(hg_arm) / sizeof(hg_arm[0]); i++)	DeleteGraph(hg_arm[i]);
	DeleteSoundMem(hs_shiftUp);
	DeleteSoundMem(hs_shiftDown);
	DeleteSoundMem(hs_dead);

	printfDx("\n~Player");
}


void Player::Update()
{
	elapsedTime++;

	switch (state)
	{
	case ePlayerState::Start: Update_Start();	break;
	case ePlayerState::Game: Update_Game();		break;
	case ePlayerState::Dead: Update_Dead();		break;
	default:	printfDx("Player.cpp:ERROR\n");	break;
	}

	isHit = false;

	/* setting some params to PShot */
	IPshot::SetParam(pos, powlv); // TODO: refactoring

	/* setting some params to bomber */
	IBomb::SetParam(pos, powlv, bombNum); // TODO: refactoring

	/* do bomb */
	const bool& canBomb = (Keyboard::Instance()->isPush(KEY_INPUT_X) && bombNum > 0 && isDead == false);
	if (canBomb)
	{
		bombNum--;
		IBomb::Fire();
	}

	// TEST------------------------------------------------------------------------------------
	if (DebugMode::isTest == false)	return;

	if (Keyboard::Instance()->isPush(KEY_INPUT_O))
		Shift(false);
	if (Keyboard::Instance()->isPush(KEY_INPUT_P))
		Shift(true);
}


void Player::Draw()
{
	/* Arm */
	if (isArm)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawAnime(pos.x, pos.y + 4., 2., 0., _countof(hg_arm), 4, hg_arm);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	/* player */
	switch (state)
	{
	case ePlayerState::Start: Draw_Start();	break;
	case ePlayerState::Game: Draw_Game();	break;
	case ePlayerState::Dead: Draw_Dead();	break;
	default:	printfDx("Player.cpp:ERROR\n");	break;
	}

	// TEST------------------------------------------------------------------------------------
	if (DebugMode::isTest == false)	return;

	DrawCircle(pos.x, pos.y + 9, HIT_RANGE, GetColor(0, 255, 0), false);
//	DrawFormatString(100, 100, GetColor(0, 255, 0), "rensha = %d", s_rensha);
//	DrawFormatString(100, 100, GetColor(0, 255, 0), "Dで死にます");
}


void Player::Update_Start()
{
	isDead = false;

	const bool& isStartPosition = (pos.y == Y_START);

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
#ifndef _DEBUG
		isMuteki = false;
#endif
		state = ePlayerState::Game;	// スタート地点ならスタートする
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
			Effector::PlayAnime(pos.x + 19., pos.y - 23., eExplosion_small);
		if (c_dead->GetNowcount() == 32)
			Effector::PlayAnime(pos.x - 3., pos.y + 68., eExplosion_small);
		if (c_dead->GetNowcount() == 24)
			Effector::PlayAnime(pos.x - 12., pos.y - 3., eExplosion_small);
		if (c_dead->GetNowcount() == 16)
			Effector::PlayAnime(pos.x + 52., pos.y - 56., eExplosion_small);
		if (c_dead->GetNowcount() == 8)
			Effector::PlayAnime(pos.x - 39., pos.y + 27., eExplosion_small);
	}

	pos += vec;

	if(c_dead->isLast())
	{
		life--;
		if (life == 0) {
			/// 死亡
			Game::GameOver();
		}

		// スタート状態にする
		state = ePlayerState::Start;

		// スタート地点に戻す
		pos.SetVec(320.0, 520.0);

		keydir = eInputDir::Neutral;

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
		case eInputDir::Left:		DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[0], true);	break;
		case eInputDir::Neutral:	DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[1], true);	break;
		case eInputDir::Right:		DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[2], true);	break;
		}
		
		SetDrawBlendMode(DX_BLENDMODE_ADD, 200); //加算合成
												 
		//Draw関数で上と同じ物を重ねる
		switch (keydir)
		{
		case eInputDir::Left:		DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[0], true);	break;
		case eInputDir::Neutral:	DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[1], true);	break;
		case eInputDir::Right:		DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[2], true);	break;
		}
		
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	else
	{
		switch (keydir)
		{
		case eInputDir::Left:		DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[0], true);	break;
		case eInputDir::Neutral:	DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[1], true);	break;
		case eInputDir::Right:		DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[2], true);	break;
		}
	}
}


void Player::Draw_Game()
{
	switch (keydir)
	{
	case eInputDir::Left:		DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[0], true);	break;
	case eInputDir::Neutral:	DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[1], true);	break;
	case eInputDir::Right:		DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[2], true);	break;
	}
}


void Player::Draw_Dead()
{
	const bool& isRed = ( elapsedTime % 4 >= 2 );
	if (isRed)	// 2fps毎に元の色に戻す
	{
		//SetDrawBlendMode(DX_BLENDMODE_INVSRC, 100);	// On-Damage-Effect
		//SetDrawBright(255, 0, 0);
		//SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);

		SetDrawBright(255, 0, 0);	// 赤色に設定
	}

	switch (keydir)
	{
	case eInputDir::Left:	DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[0], true);	break;
	case eInputDir::Neutral:	DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[1], true);	break;
	case eInputDir::Right:	DrawRotaGraph(pos.x, pos.y, 2.0, 0.0, hg[2], true);	break;
	}

	SetDrawBright(255, 255, 255);	// 元の色合い
}


void Player::SetStart()
{
	if (isStart)	return;

	const bool& isStartPosition = (pos.y == Y_START);

	c_start->Update();
	pos.y -= 2.0;								// 上に上昇
	pos.y = std::max(pos.y, Y_START);			// スタート地点まで

	if (isStartPosition)
		isStart = true;	// スタート地点ならスタートする
}


void Player::InputMng()
{
	const bool& KEY_STAY_RIGHT = ( Keyboard::Instance()->GetDown(KEY_INPUT_RIGHT) >= 30 );
	const bool& KEY_STAY_LEFT = (Keyboard::Instance()->GetDown(KEY_INPUT_LEFT) >= 30 );
	const bool& PUSH_Z_KEY = (Keyboard::Instance()->isPush(KEY_INPUT_Z));

	keydir = eInputDir::Neutral;						// キーをニュートラルにする
	
	if (KEY_STAY_RIGHT)	keydir = eInputDir::Right;

	if (KEY_STAY_LEFT)	keydir = eInputDir::Left;

	if (PUSH_Z_KEY && powlv == 0)	Shift(true);	// 無の状態で押すと無条件で１段階アップ
}


void Player::Move()
{
	const bool& INPUT_HORIZONTAL  = Keyboard::Instance()->isDown(KEY_INPUT_RIGHT) ||
									Keyboard::Instance()->isDown(KEY_INPUT_LEFT);
	const bool& INPUT_VERTICAL    = Keyboard::Instance()->isDown(KEY_INPUT_UP) ||
									Keyboard::Instance()->isDown(KEY_INPUT_DOWN);
	const bool& KEY_STAY_RIGHT    = Keyboard::Instance()->isDown(KEY_INPUT_RIGHT);
	const bool& KEY_STAY_LEFT     = Keyboard::Instance()->isDown(KEY_INPUT_LEFT);
	const bool& KEY_STAY_UP       = Keyboard::Instance()->isDown(KEY_INPUT_UP);
	const bool& KEY_STAY_DOWN     = Keyboard::Instance()->isDown(KEY_INPUT_DOWN);
	
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


void Player::Rensha_Update()
{
	/* 連射ゲージ加算 */
	const bool& isRensha = (Keyboard::Instance()->isPush(KEY_INPUT_Z) ||
							Keyboard::Instance()->isPush(KEY_INPUT_A));
	if(isRensha)
	{
		if(rensha < 41)
			rensha++;
	}

	/* 連射ゲージの減少速度計算 */
	switch (powlv)
	{
	case 0:	rensha = 0;	break;
	case 1:	if (IStage::GetTime() % 17 == 0)	rensha--;	break;
	case 2:	if (IStage::GetTime() % 12 == 0)	rensha--;	break;
	case 3: if (IStage::GetTime() % 11 == 0)	rensha--;	break;
	case 4: if (IStage::GetTime() % 10 == 0)	rensha--;	break;
	default: assert(!"Player::Rensha_Update()");
	}

	/* シフトチェンジ判定 */
	if(rensha == 0)
	{
		if (powlv != 0)
			Shift(false);
	}
	else if(rensha == 40)
	{
		if (powlv != 4)
			Shift(true);
	}
}


void Player::Death()
{
	powlv = 0;
	state = ePlayerState::Dead;
	isHit = true;
	isMuteki = true;
	isDead = true;
	vec.SetVec(std::cos(1.5 * GetRand(100)), 1.5 * std::cos(GetRand(100)));
	Effector::PlaySpread(pos.x, pos.y, GetRand(100), dead_ef);
	IStage::Quake();
	PlaySoundMem(hs_dead, DX_PLAYTYPE_BACK);
}


void Player::SetArm()
{
	if(isArm)
	{
		IScore::AddScore(10000);
		return;
	}
	isArm = true;
}


void Player::Shift(const bool isUP)
{
	if (isUP) {
		powlv++;	// 弾レベルを１段階パワーアップ
		if (powlv != 1)
		{
			rensha = 1 + (2 * powlv);	// シフトアップボーナス４メモリ
			PlaySoundMem(hs_shiftUp, DX_PLAYTYPE_BACK);
		}
	} else {
		powlv--;
		rensha = MAX_RENSHA - 4;
		if (powlv == 0)	rensha = 0;
		if (powlv != 0)	PlaySoundMem(hs_shiftDown, DX_PLAYTYPE_BACK);
	}

	powlv = std::min(MaxPowLevel, std::max(0, powlv));	// レベルを基底の範囲内にする
}


void Player::DownBombNum()
{
	if (bombNum == 0)	return;
	bombNum--;
}


bool Player::HitCheckCircle(const double& ColX, const double& ColY)
{
	if (state == ePlayerState::Dead)
		return false;

	const bool& IS_HIT = Vector2D::CirclePointCollision(pos.x, pos.y + 9.0, ColX, ColY, HIT_RANGE);

	if (IS_HIT && state == ePlayerState::Game && isMuteki == false)
		Death();

	return IS_HIT;
}


bool Player::HitCheckCircle(const double & Range1, const double & Range2, const double & X1, const double & Y1, const double & X2, const double & Y2)
{
	if (state == ePlayerState::Dead)
		return false;

	const bool& IS_HIT = Vector2D::CirclesCollision(Range1, Range2, X1, Y1, X2, Y2);

	// アームを剥がす
	if (IS_HIT && isArm)
		isArm = false;

	if (IS_HIT && state == ePlayerState::Game && isMuteki == false)
		Death();

	return IS_HIT;
}


// IPlayer===================================================
std::shared_ptr<Player> IPlayer::mPlayer;


void IPlayer::set(std::shared_ptr<Player> player)
{
	mPlayer = player;
	assert(mPlayer != nullptr && "IPlayer::set()");
}
