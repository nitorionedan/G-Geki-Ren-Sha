#include "BossA.hpp"
#include "Keyboard.hpp"
#include "DebugMode.hpp"
#include "Eshot.hpp"
#include "Vector2D.hpp"
#include "Bomb.hpp"
#include "IScore.hpp"
#include "ChargeEffect.hpp"
#include "ExplosionEffect.hpp"
#include "Effector.hpp"
#include "PieceEffect.hpp"
#include "Stage.hpp"

#include <DxLib.h>
#include <cmath>
#include <algorithm>

#undef min
#undef max


constexpr int DEAD_TIME = 530;
constexpr int StartStayTime = 600;
constexpr double HitBrake = 0.6;

const float BossA::SC_LIMIT_XL = 78.f;
const float BossA::SC_LIMIT_XR = 560.f;
const float BossA::SC_LIMIT_YT = 74.f;
const float BossA::SC_LIMIT_YB = 406.f;
const float BossA::HIT_RANGE   = 60.f;
const int BossA::MAX_HP        = 1500; // 3000 << too many

eBossA_state BossA::state;
int BossA::hp;
int BossA::time;
int BossA::hs_break;
bool BossA::isExist;
bool BossA::isHit;
bool BossA::isWeak;


BossA::BossA()
	: SPEED(2.f)
	, c_starting(new Counter(3000)) // about 5sec
	, c_end(new Counter(18000))
	, c_atk1(new Counter(370))
	, shot3(new EShot03)
	, bomber(new Effect(new ChargeEffect(100, 10, 70, 10)))		// 200, 10, 70, 10
	, big_time(0)
	, angle(0.0f)
	, isMove(false)
	, isEnd(false)
{
	hg_shield = LoadGraph("GRAPH/GAME/ENEMY/Shield.png");
	hs_big = LoadSoundMem("SOUND/SE/eshot03.wav");
	hs_exp = LoadSoundMem("SOUND/SE/explosion03.mp3");
	hs_break = LoadSoundMem("SOUND/SE/break00.wav");
	hm = MV1LoadModel("GRAPH/MODEL/BossA_3.x");
	hmWeaking = MV1LoadModel("GRAPH/MODEL/BossA_3.x");
	hm_core = MV1LoadModel("GRAPH/MODEL/ring.x");
	Screen = MakeScreen(640, 480, TRUE);

	mPos = VGet(320.f, 300.f, -300.f);
	rota = VGet(1.5f, 0.f, 0.f);
	rota_core = VGet(0.f, 0.f, 0.f);

	c_dead = 0;
	c_start = 0;

	MV1SetRotationXYZ(hm, rota);				// 回転値を設定
	MV1SetPosition(hm, mPos);					// 座標を設定
	MV1SetScale(hm, VGet(13.f, 13.f, 13.f));	// モデル拡大
	MV1SetRotationXYZ(hmWeaking, rota);				// 回転値を設定
	MV1SetPosition(hmWeaking, mPos);					// 座標を設定
	MV1SetScale(hmWeaking, VGet(13.f, 13.f, 13.f));	// モデル拡大
	MV1SetScale(hm_core, VGet(5.f, 5.f, 5.f));	// モデル拡大

	pos = Vector2D::ZERO;
	state = eBossA_Start;
	hp = MAX_HP;
	time   = 0;
	pos.SetVec(400.f, -100.f);
	isExist = true;
	isHit  = false;
	isDead   = false;
	isWeak = false;

	startPos = ConvScreenPosToWorldPos(VGet(pos.x, pos.y, 0.5f));	// スクリーン座標からワールド座標へ
	mPos = startPos;												// モデルをスクリーン座標にあてがる
	MV1SetWireFrameDrawFlag(hmWeaking, TRUE);
}


BossA::~BossA()
{
	DeleteGraph(hg_shield);
	DeleteSoundMem(hs_big);
	DeleteSoundMem(hs_exp);
	DeleteSoundMem(hs_break);
	MV1DeleteModel(hm);
	MV1DeleteModel(hmWeaking);
	MV1DeleteModel(hm_core);
}


void BossA::Update()
{
	if(state != eBossA_Dead)
		shot3->Update(pos.x, pos.y);

	if (!isExist)	return;

	time++;	// 経過時間加算
	
	if (!isDead)	c_end->Update();	

	rota.z += 0.03f;												// モデルを回転

	if (isHit)
		rota_core.x += 0.005f;
	else
		rota_core.x += 0.1f;

	startPos = ConvScreenPosToWorldPos(VGet(pos.x, pos.y, 0.5f));	// スクリーン座標からワールド座標へ
	mPos = startPos;												// モデル座標をスクリーン座標系に調節

	switch (state)
	{
	case eBossA_Start:	Start_Update();		break;
	case eBossA_Normal:	Normal_Update();	break;
	case eBossA_Weak:	Weak_Update();		break;
	case eBossA_Dead:	Dead_Update();		break;
	case eBossA_End:	End_Update();		break;
	default:	break;
	}

	isHit = false;

	HitCheck();

	/* エフェクト系 */
	bomber->Update(pos.x, pos.y);

	/*
	if (!isMove)
	{
		s_isHit = false;
		StartMove();
		return;
	}

	Move1();	// 動き
	AttackMng();

	if (hp <= 0)	isDead = true;
	c_atk1->Update();

	s_isHit = false;
	*/


	// TEST-------------------------------------------------------------------------------------
	if (DebugMode::isTest == false)	return;
}


void BossA::Draw()
{
	if (isExist == false)
		return;

	if(isHit)
		SetLightDifColor(CyanF);	// 緑
	
	/* flashing */
	const bool& Is_flashing = (isWeak && (time % 12) >= 10);
	if(Is_flashing)
		SetLightDifColor(RedF);

	/* preliminary action */
	const bool&  PreliminaryActionFlag = (c_atk1->GetNowcount() == 50);
	if(PreliminaryActionFlag)
		SetLightDifColor(YellowF);	// 色変更
	
	if (isWeak)
	{
		MV1SetRotationXYZ(hmWeaking, rota);	// 回転値設定
		MV1SetPosition(hmWeaking, mPos);	// 座標を設定
		MV1SetRotationXYZ(hm_core, rota_core);	// 回転値設定
		MV1SetPosition(hm_core, mPos);	// 座標を設定

		MV1DrawModel(hm_core);
		MV1DrawModel(hmWeaking);
	}
	else
	{
		MV1SetRotationXYZ(hm, rota);	// 回転値設定
		MV1SetPosition(hm, mPos);		// 座標を設定
		MV1DrawModel(hm);
	}

	SetLightDifColor(GetColorF(1.f, 1.f, 1.f, 1.f));	// 元の色

	const bool& Is_NoGuard = (isHit && !isWeak);
	if (Is_NoGuard)
		DrawRotaGraph(pos.x, pos.y, 2.5, GetRand(100), hg_shield, true);

	/* エフェクト関連 */
	bomber->Draw();

	/* 弾 */
	shot3->Draw();

	// TEST-------------------------------------------------------------------
	if (DebugMode::isTest == false)	return;
	
	DrawCircle(pos.x, pos.y, HIT_RANGE, GetColor(0, 255, 0), false);
	DrawFormatString(pos.x, pos.y, GetColor(0, 255, 0), "HP : %d", hp);
}


int BossA::GetTime() { return time; }


bool BossA::HitCheck(const double & ColX, const double & ColY, const int& DamagePoint)
{
	if (state == eBossA_Dead)
		return false;

	const bool& IS_HIT = (Vector2D::CirclePointCollision(pos.x, pos.y, ColX, ColY, HIT_RANGE));
	
	if (IS_HIT)
	{
		isHit = true;
		Damage(DamagePoint);
		IScore::AddScore(1);
	}

	return IS_HIT;
}


void BossA::ChangeState(eBossA_state state_) { state = state_; }


void BossA::Start_Update()
{
	if (c_start < StartStayTime)
		++c_start;

	if (c_start != StartStayTime)
		return;

	pos.x -= SPEED;
	pos.y += 1.5f;
	
	if (pos.x < SC_LIMIT_XL)	ChangeState(eBossA_Normal);
}


void BossA::Normal_Update()
{
	const float& tarX = IPlayer::GetPos().x;
	const float& tarY = IPlayer::GetPos().y;

	// 自機の方向を計算
	if (isOverLimit())
		angle = std::atan2(tarY - pos.y, tarX - pos.x);

	// 弾に当たったときスピードを落とす
	if(isHit)
	{
		pos.x += std::cos(angle) * SPEED * HitBrake;
		pos.y += std::sin(angle) * SPEED * HitBrake;
	} else {
		pos.x += std::cos(angle) * SPEED;
		pos.y += std::sin(angle) * SPEED;
	}

	if (c_atk1->isLast())	Eshot::GoFire1();
	
	c_atk1->Update();
}


void BossA::Weak_Update()
{
	big_time++;

	const float& playerX = IPlayer::GetPos().x;
	const float& playerY = IPlayer::GetPos().y;

	// 自機の方向を計算
	if (isOverLimit())
		angle = std::atan2(playerY - pos.y, playerX - pos.x);

	// 弾に当たったときスピードを落とす
	if (isHit)
	{
		pos.x += std::cos(angle) * SPEED * HitBrake;
		pos.y += std::sin(angle) * SPEED * HitBrake;
	} 
	else
	{
		pos.x += std::cos(angle) * SPEED;
		pos.y += std::sin(angle) * SPEED;
	}

	if (c_atk1->isLast())
		Eshot::GoFire1();
	
	// チャージ
	if (big_time == 400)
		bomber->PlayAnime(pos.x, pos.y);

	// 破片
	if (big_time == 500)
		Effector::PlaySpread(pos.x, pos.y, GetRand(100), eSpread_SmallOrange);
	
	// 大きい弾
	if (big_time == 500)
	{
		PlaySoundMem(hs_big, DX_PLAYTYPE_BACK);
		Effector::PlayShock(pos.x, pos.y);
		Effector::PlaySpread(pos.x, pos.y, GetRand(100), eSpread_SmallOrange);
		shot3->Fire(10, std::atan2(playerY - pos.y, playerX - pos.x));
	}

	// 大きい弾
	if(big_time == 530)
	{
		PlaySoundMem(hs_big, DX_PLAYTYPE_BACK);
		Effector::PlayShock(pos.x, pos.y);
		Effector::PlaySpread(pos.x, pos.y, GetRand(100), eSpread_SmallOrange);
		shot3->Fire(10, std::atan2(playerY - pos.y, playerX - pos.x));
	}

	if(big_time == 530)	big_time = 0;

	c_atk1->Update();
}


void BossA::Dead_Update()
{
	c_dead++;

	const bool& Is_deing = (time % 20 == 0 && pos.y < 480);
	if(Is_deing)	// small explosion
		Effector::PlayAnime(pos.x + GetRand(80) - 40, pos.y + GetRand(80) - 40, eExplosion_small);

	pos.y += 1.;	// falling slowly

	if (c_dead == 180.)	// final explosion
	{
		isExist = false;

		for (int i = 0; i < 5; ++i)
			Effector::PlayAnime(pos.x, pos.y, eExplosion_big);
		
		PlaySoundMem(hs_exp, DX_PLAYTYPE_BACK);

		IStage::Clear();
	}
}


void BossA::End_Update()
{
}


bool BossA::isFine(){
	return (hp >= MAX_HP / 3) ? true : false;
}


void BossA::HitCheck()
{
	const bool& Is_hitBomb = IBomb::IsHit(HIT_RANGE, pos.x, pos.y);

	if(Is_hitBomb && isWeak)
	{
		isHit = true;
		Damage(5);
	}

	// player's collision
	if (state == eBossA_Normal || state == eBossA_Weak)
	{
		const bool& Is_hitPlayer = IPlayer::HitCheckCircl(HIT_RANGE, pos);
		if (Is_hitPlayer)	Damage(1);
	}
}


bool BossA::isOverLimit()
{
	const bool& IS_OUT = (pos.x < SC_LIMIT_XL ||
						  SC_LIMIT_XR < pos.x ||
						  pos.y < SC_LIMIT_YT ||
						  SC_LIMIT_YB < pos.y);

	// 画面外に出てしまったら
	if (IS_OUT)
	{
		pos.x = std::min(std::max(pos.x, static_cast<double>(SC_LIMIT_XL)), static_cast<double>(SC_LIMIT_XR) );
		pos.y = std::min(std::max(pos.y, static_cast<double>(SC_LIMIT_YT)), static_cast<double>(SC_LIMIT_YB));
	}

	return IS_OUT;
}


void BossA::Damage(const int& point)
{
	// スタート中ならここで返す
	if (state == eBossA_Start)	return;


	const bool& IS_ALIVE = (hp > 0);
	
	if (IS_ALIVE)	// 生きていれば
		hp -= point;

	const bool& IS_FINE = (hp > MAX_HP / 3);
	const bool& IS_WEAK = (!isWeak && !IS_FINE);
	
	if (IS_WEAK)	// 弱っていれば
	{
		Effector::PlayAnime(pos.x + 40, pos.y + 40, eExplosion_small);
		Effector::PlayAnime(pos.x + 20, pos.y - 10, eExplosion_small);
		Effector::PlayAnime(pos.x - 20, pos.y + 20, eExplosion_small);

		Effector::PlaySpread(pos.x, pos.y, GetRand(100), eSpread_SmallBlue);
		PlaySoundMem(hs_break, DX_PLAYTYPE_BACK);
	}
	
	if (!IS_FINE && state != eBossA_Weak)	// HPが3分の1以下になると酔った状態になる
	{
		ChangeState(eBossA_Weak);
		isWeak = true;
	}

	const bool& IS_DEAD = (hp <= 0);
	
	if (IS_DEAD)
	{
		ChangeState(eBossA_Dead);
		c_dead = 0;
		isDead = true;
		Effector::PlayAnime(pos.x, pos.y, eExplosion_big);
		Effector::PlaySpread(pos.x, pos.y, GetRand(100), eSpread_BigAll);
		Effector::PlaySpread(pos.x, pos.y, GetRand(100), eSpread_BigAll);
	}
}