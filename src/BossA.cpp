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

#include <DxLib.h>
#include <cmath>
#include <algorithm>

#undef min
#undef max


static const int DEAD_TIME = 530;

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
	hm		 = MV1LoadModel("GRAPH/MODEL/BossA_2.x");

	mPos.x = 320.f;
	mPos.y = 300.f;
	mPos.z = -300.f;

	rota.x = 1.5f;
	rota.y = 0;
	rota.z = 0;

	MV1SetRotationXYZ(hm, rota);				// ��]�l��ݒ�
	MV1SetPosition(hm, mPos);					// ���W��ݒ�
	MV1SetScale(hm, VGet(13.f, 13.f, 13.f));	// ���f���g��

	// static -----------------------------------------------------
	pos = Vector2D::ZERO;
	state = eBossA_Start;
	hp = MAX_HP;
	time   = 0;
	pos.x	 = 400.0f;
	pos.y	 = -100.0f;
	isExist = true;
	isHit  = false;
	isDead   = false;
	isWeak = false;

	startPos = ConvScreenPosToWorldPos(VGet(pos.x, pos.y, 0.5f));	// �X�N���[�����W���烏�[���h���W��
	mPos = startPos;												// ���f�����X�N���[�����W�ɂ��Ă���
}


BossA::~BossA()
{
	DeleteGraph(hg_shield);
	DeleteSoundMem(hs_big);
	DeleteSoundMem(hs_exp);
	DeleteSoundMem(hs_break);
	MV1DeleteModel(hm);
}


void BossA::Update()
{
	if(state != eBossA_Dead)
		shot3->Update(pos.x, pos.y);

	if (!isExist)	return;

	time++;	// �o�ߎ��ԉ��Z
	
	if (!isDead)	c_end->Update();	

	rota.z += 0.03f;												// ���f������]
	startPos = ConvScreenPosToWorldPos(VGet(pos.x, pos.y, 0.5f));	// �X�N���[�����W���烏�[���h���W��
	mPos = startPos;												// ���f�����W���X�N���[�����W�n�ɒ���

	isHit = false;

	switch (state)
	{
	case eBossA_Start:	Start_Update();		break;
	case eBossA_Normal:	Normal_Update();	break;
	case eBossA_Weak:	Weak_Update();		break;
	case eBossA_Dead:	Dead_Update();		break;
	case eBossA_End:	End_Update();		break;
	default:	break;
	}

	HitCheck();

	if (state != eBossA_Start)
	{
		const bool& IS_HIT = IPlayer::HitCheckCircl(HIT_RANGE, pos);
		if (IS_HIT)	Damage(10);
	}

	/* �G�t�F�N�g�n */
	bomber->Update(pos.x, pos.y);

	/*
	if (!isMove)
	{
		s_isHit = false;
		StartMove();
		return;
	}

	Move1();	// ����
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
	const bool& Is_NoGuard = (isHit && !isWeak);
	MV1SetWireFrameDrawFlag(hm, !Is_NoGuard);

	if(isHit)
		SetLightDifColor(CyanF);	// ��
	
	const bool& Is_flashing = (isWeak && (time % 12) >= 10);
	if(Is_flashing)
		SetLightDifColor(RedF);

	const bool&  PreliminaryActionFlag = (c_atk1->GetNowcount() == 50);
	if(PreliminaryActionFlag)
		SetLightDifColor(YellowF);	// �F�ύX

	MV1SetRotationXYZ(hm, rota);	// ��]�l�ݒ�
	MV1SetPosition(hm, mPos);		// ���W��ݒ�
	MV1DrawModel(hm);				// 3D�I�u�W�F�N�g�`��

	SetLightDifColor(GetColorF(1.f, 1.f, 1.f, 1.f));	// ���̐F

	if (Is_NoGuard)
		DrawRotaGraph(pos.x, pos.y, 2.5, GetRand(100), hg_shield, true);

	/* �G�t�F�N�g�֘A */
	bomber->Draw();

	/* �e */
	shot3->Draw();

	// TEST-------------------------------------------------------------------
	if (DebugMode::isTest == false)	return;
	
	DrawCircle(pos.x, pos.y, HIT_RANGE, GetColor(0, 255, 0), false);
	DrawFormatString(pos.x, pos.y, GetColor(0, 255, 0), "%d", hp);
}


int BossA::GetTime() { return time; }


bool BossA::HitCheck(const double & ColX, const double & ColY, const int& DamagePoint)
{
	if (!isExist)	return false;

	const bool& IS_HIT = (Vector2D::CirclePointCollision(pos.x, pos.y, ColX, ColY, HIT_RANGE));
	
	if (IS_HIT)	isHit = true;
	
	if (IS_HIT)
	{
		Damage(DamagePoint);
		IScore::AddScore(1);
	}

	return IS_HIT ? true : false;
}


void BossA::ChangeState(eBossA_state state_) { state = state_; }


void BossA::GetPosition(double* x, double* y)
{
	*x = this->pos.x;
	*y = this->pos.y;
}


void BossA::Start_Update()
{

	pos.x -= SPEED;
	pos.y += 1.5f;
	
	if (pos.x < SC_LIMIT_XL)	ChangeState(eBossA_Normal);
}


void BossA::Normal_Update()
{
	const float& tarX = IPlayer::GetPos().x;
	const float& tarY = IPlayer::GetPos().y;

	// ���@�̕������v�Z
	if (isOverLimit())
		angle = std::atan2(tarY - pos.y, tarX - pos.x);

	// �e�ɓ��������Ƃ��X�s�[�h�𗎂Ƃ�
	if(isHit)
	{
		pos.x += std::cos(angle) * SPEED * 0.6;
		pos.y += std::sin(angle) * SPEED * 0.6;
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

	// ���@�̕������v�Z
	if (isOverLimit())
		angle = std::atan2(playerY - pos.y, playerX - pos.x);

	// �e�ɓ��������Ƃ��X�s�[�h�𗎂Ƃ�
	if (isHit)
	{
		pos.x += std::cos(angle) * SPEED * 0.6;
		pos.y += std::sin(angle) * SPEED * 0.6;
	} else {
		pos.x += std::cos(angle) * SPEED;
		pos.y += std::sin(angle) * SPEED;
	}

	if (c_atk1->isLast())	Eshot::GoFire1();
	
	// �`���[�W
	if (big_time == 400)	bomber->PlayAnime(pos.x, pos.y);

	// �j��
	if (big_time == 500)	Effector::PlaySpread(pos.x, pos.y, GetRand(100), eSpread_SmallOrange);
	
	// �傫���e
	if (big_time == 500)
	{
		PlaySoundMem(hs_big, DX_PLAYTYPE_BACK);
		shot3->Fire(10, std::atan2(playerY - pos.y, playerX - pos.x));
	}

	// �傫���e
	if(big_time == 530)
	{
		PlaySoundMem(hs_big, DX_PLAYTYPE_BACK);
		shot3->Fire(10, std::atan2(playerY - pos.y, playerX - pos.x));
	}

	if(big_time == 530)	big_time = 0;

	c_atk1->Update();
}


void BossA::Dead_Update()
{
	static int count = 0;
	count++;

	// ���̊Ԋu�ŏ�����
	if(time % 20 == 0 && pos.y < 480)	Effector::PlayAnime(pos.x + GetRand(80) - 40, pos.y + GetRand(80) - 40, eExplosion_small);

	pos.y += 1.;

	// �w�肵�����Ԃɔ�����
	if(count == 180)
	{
		PlaySoundMem(pos.x, pos.y, DX_PLAYTYPE_BACK);
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
	const bool& IS_HIT = IBomb::IsHit(HIT_RANGE, pos.x, pos.y);


	if(IS_HIT && isWeak)
	{
		isHit = true;
		Damage(100);
	}
}


bool BossA::isOverLimit()
{
	const bool& IS_OUT = (pos.x < SC_LIMIT_XL || SC_LIMIT_XR < pos.x || pos.y < SC_LIMIT_YT || SC_LIMIT_YB < pos.y);

	// ��ʊO�ɏo�Ă��܂�����
	if (IS_OUT)
	{
		pos.x = std::min(std::max((float)pos.x, SC_LIMIT_XL), SC_LIMIT_XR);
		pos.y = std::min(std::max((float)pos.y, SC_LIMIT_YT), SC_LIMIT_YB);
	}

	return IS_OUT;
}


void BossA::Damage(const int& point)
{
	// �X�^�[�g���Ȃ炱���ŕԂ�
	if (state == eBossA_Start)	return;

	const bool& IS_ALIVE = (hp > 0);
	const bool& IS_FINE = (hp > MAX_HP / 3);
	const bool& IS_WEAK = (!isWeak && !IS_FINE);
	const bool& IS_DEAD = (hp <= 0);

	// �����Ă����
	if (IS_ALIVE)
		hp -= point;
	
	// ����Ă����
	if (IS_WEAK)
	{
		Effector::PlayAnime(pos.x + 40, pos.y + 40, eExplosion_small);
		Effector::PlayAnime(pos.x + 20, pos.y - 10, eExplosion_small);
		Effector::PlayAnime(pos.x - 20, pos.y + 20, eExplosion_small);

		Effector::PlaySpread(pos.x, pos.y, GetRand(100), eSpread_SmallBlue);
		PlaySoundMem(hs_break, DX_PLAYTYPE_BACK);
	}
	
	// HP��3����1�ȉ��ɂȂ�Ɛ�������ԂɂȂ�
	if (!IS_FINE && state != eBossA_Weak)
	{
		ChangeState(eBossA_Weak);
		isWeak = true;
	}

	// ����ł����
	if (IS_DEAD)
	{
		ChangeState(eBossA_Dead);
		isDead = true;
		Effector::PlayAnime(pos.x, pos.y, eExplosion_big);
		Effector::PlaySpread(pos.x, pos.y, GetRand(100), eSpread_BigAll);
		Effector::PlaySpread(pos.x, pos.y, GetRand(100), eSpread_BigAll);
	}
}