/* 
@brief  Define of Enemy.hpp
@author Shohei
*/
#include "Enemy.hpp"
#include "Graphics2D.hpp"
#include "DebugMode.hpp"
#include "EnemyMng.hpp"
#include "IScore.hpp"
#include "Effector.hpp"
#include "ExplosionEffect.hpp"
#include "PieceEffect.hpp"
#include "ItemMng.hpp"
#include "Keyboard.hpp"
#include "Stage.hpp"

// Shots
#include "EShot00.hpp"
#include "EShot01.hpp"
#include "EShot02.hpp"
#include "Eshot04.hpp"
#include "EneShot.hpp"

#include <DxLib.h>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <functional>

#undef max // ざけんなこのカス必要ねえんだよ！
#undef min // wtf

namespace
{
	auto RadToAng = [](double rad) {
		return rad * 180 / DX_PI;
	};

	auto AngToRad = [](double ang) {
		return ang * DX_PI / 180;
	};
}


Enemy::Enemy(tEnemyData param)
	: SCREEN_LIMIT_XL(-10)
	, SCREEN_LIMIT_XR(650)
	, SCREEN_LIMIT_YT(-10)
	, SCREEN_LIMIT_YB(490)
	, MAX_HP(param.hp)
	, DROP_RATE(2)
	, shot(nullptr)
	, shot2(nullptr)
	, shot3(nullptr)
	, SPEED_0(4.)
	, gh_shot00(NULL)
	, sh_voice(NULL)
	, elapsedTime(0)
	, moveTime(0)
	, s_time(0)
	, angle(0.)
	, isExist(false)
	, isMove(true)
	, isDamage(false)
	, isUngry(false)
{
	/* itemID check */
	if (param.item < 0 || param.item > 3)
		assert(!"out of range");

	// データ設定
	this->param = param;
	pos.SetVec(static_cast<double>(param.x_pos), static_cast<double>(param.y_pos));

	// タイプに合わせた
	switch (this->param.type)
	{
	case 0:
		gh_ene00 = LoadGraph("GRAPH/GAME/ENEMY/ene00.png");
		hitRange = 20.;
		break;
	case 1:
		break;
	case 2:
		LoadDivGraph("GRAPH/GAME/ENEMY/ene02.png", 4, 4, 1, 31, 16, gh_ene02);
		hitRange = 20.;
		hitSRange.SetVec(40., 40.);
		break;
	case 3:
		break;
	case 4:
		LoadDivGraph("GRAPH/GAME/ENEMY/career.png", 8, 4, 2, 124, 78, gh_ene04);
		sh_voice = LoadSoundMem("SOUND/SE/scream00.wav");
		hitRange = 50.;
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	}

	// 速度
	switch (param.m_pattern)
	{
	case 0:	vSpeed.SetVec(2., 4.);	break;
	case 1:	break;
	case 2:	vSpeed.SetVec(2., 0.1);	break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	}

	// 弾ロード
	switch (param.s_pattern)
	{
	case 0:	shot = new Shot(new EShot00);	break;
	case 1:	break;
	case 2:	shot = new Shot(new EShot00);	break;
	case 3:	break;
	case 4:
		shot = new Shot(new EShot04);
		shot2 = new Shot(new EShot01);	break;
	case 5:	break;
	case 6:	break;
	case 7:	break;
	default: assert(!"Enemy.cpp:ERROR");	break;
	}

	gh_shot00 = LoadGraph("GRAPH/GAME/Eshot/efire0.png");
}


Enemy::~Enemy()
{
	delete shot;
	delete shot2;
	delete shot3;

	// 画像アンロード
	switch (param.type)
	{
	case 0:
		DeleteGraph(gh_ene00);
		break;
	case 1:
		break;
	case 2:
		for (int i = 0; i < _countof(gh_ene02); ++i)
			DeleteGraph(gh_ene02[i]);
		break;
	case 3:
		break;
	case 4:
		for (int i = 0; i < _countof(gh_ene04); ++i)
			DeleteGraph(gh_ene04[i]);
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;

	default:	assert("Enemy::~Enemy()");
	}

	if (gh_shot00 != NULL)	DeleteGraph(gh_shot00);
	if (sh_voice != NULL)	DeleteSoundMem(sh_voice);
}


void Enemy::Update()
{
	if (IEnemyMng::GetEneNum() != 0) // TODO: なんとかしてくれ
	{
		// 登場時間が来たら出てくる
		if (IStage::GetTime() == param.in_time)
			isExist = true;

		if (isExist)
		{
			++elapsedTime;		// 敵にとっての時間を経過させる
			++s_time;			// 全体としての時間

			isDamage = false;

			// 攻撃されたら怒る
			if (Keyboard::Instance()->isPush(KEY_INPUT_Z))
				isUngry = true;

			Move();
		}
	}

	shot->Update(pos.x, pos.y);
	
	if (shot2 != nullptr)
		shot2->Update(pos.x, pos.y);
	
	if (shot3 != nullptr)	
		shot3->Update(pos.x, pos.y);

	if (!isExist)	return;
	
	Fire();
}


void Enemy::Draw()
{
	if (isExist)
	{
		// On-Damage-Effect
		if (MAX_HP / 3 >= param.hp && elapsedTime % 12 >= 10)
		{
			SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255);
			SetDrawBright(255, 0, 0);
		}

		if (isDamage)
			SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255);

		switch (param.type)
		{
		case 0:
			DrawRotaGraph(pos.x, pos.y, 2., angle, gh_ene00, true);
			break;
		case 1:
			break;
		case 2:
			DrawAnime(pos.x, pos.y, 2., angle, elapsedTime, _countof(gh_ene02), 2, gh_ene02);
			break;
		case 3:
			break;
		case 4:
			DrawAnime(pos.x, pos.y, 2., 0., elapsedTime, _countof(gh_ene04), 8, gh_ene04);
			if (s_time == param.stop_time ||
				s_time == param.stop_time + 20 ||
				s_time == param.stop_time + 40)
			{
				DrawRotaGraph(pos.x + 70, pos.y - 30, 2.0, 0.0, gh_shot00, true);
				DrawRotaGraph(pos.x + 50, pos.y, 2.0, 0.0, gh_shot00, true);
				DrawRotaGraph(pos.x - 70, pos.y - 30, 2.0, 0.0, gh_shot00, true);
				DrawRotaGraph(pos.x - 50, pos.y, 2.0, 0.0, gh_shot00, true);
			}
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		SetDrawBright(255, 255, 255);
	}

	shot->Draw();
	if (shot2 != nullptr)	shot2->Draw();
	if (shot3 != nullptr)	shot3->Draw();

	// TEST
	if (!DebugMode::isTest)	return;

	if (!isExist)			return;

	//	DrawFormatString(300, 300, GetColor(0, 255, 0), "%lf", angle);
	DrawCircle(pos.x, pos.y, hitRange, GetColor(0, 255, 0), false);
}

const int Enemy::GetParam(eEnemyParam param)
{
	int tmpParam = NULL;
	switch (param)
	{
	case eEnemyParam::type:	     return this->param.type;      break;
	case eEnemyParam::stype:     return this->param.stype;	   break;
	case eEnemyParam::m_pattern: return this->param.m_pattern; break;
	case eEnemyParam::s_pattern: // I'm tired...
		return this->param.s_pattern;
		break;
	case eEnemyParam::in_time:
		return this->param.in_time;
		break;
	case eEnemyParam::stop_time:
		return this->param.stop_time;
		break;
	case eEnemyParam::shot_time:
		return this->param.shot_time;
		break;
	case eEnemyParam::out_time:
		return this->param.out_time;
		break;
	case eEnemyParam::x_pos:
		return this->param.x_pos;
		break;
	case eEnemyParam::y_pos:
		return this->param.y_pos;
		break;
	case eEnemyParam::s_speed:
		return this->param.s_speed;
		break;
	case eEnemyParam::hp:
		return this->param.hp;
		break;
	case eEnemyParam::item:
		return this->param.item;
		break;
	default:
		assert(!"abnormality param");
	}

	return NULL;
}


bool Enemy::IsHit(const double & ColX, const double & ColY, const int& DAMAGE)
{
	if (!isExist)	return false;

	bool isHit;

	switch (param.type)
	{
	case 0:
		isHit = Vector2D::CirclesCollision(hitRange, 6, pos.x, pos.y + 9., ColX, ColY);	
		break;
	case 1:
		break;
	case 2:
		isHit = Vector2D::CirclesCollision(hitRange, 6, pos.x, pos.y + 9., ColX, ColY);
		break;
	case 3:
		break;
	case 4:
		isHit = Vector2D::CirclesCollision(hitRange, 6, pos.x, pos.y + 9., ColX, ColY);
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	}

	// ダメージ処理
	if (isHit)
		Damage(DAMAGE);

	// 全部falseならここにくる
	return isHit;
}


bool Enemy::IsHit(const int & ColCircle, const double & ColX, const double & ColY, const int & Damage)
{
	if (!isExist)	return false;

	const bool& IS_HIT = Vector2D::CirclesCollision(ColCircle, hitRange, ColX, ColY, pos.x, pos.y);
	
	if (IS_HIT)	this->Damage(Damage);
	
	return IS_HIT;
}


void Enemy::AngleTarget(const double Target_x, const double Target_y)
{
	angle = atan2(Target_y - pos.y, Target_x - pos.x) - DX_PI / 2;	// 自機に向く
}


void Enemy::Move()
{
	switch (param.m_pattern)
	{
	case 0:	Move_0();	break;
	case 1:	Move_1();	break;
	case 2:	Move_2();	break;
	case 3:	Move_3();	break;
	case 4:	Move_4();	break;
	case 5:	Move_5();	break;
	case 6:	Move_6();	break;
	case 7:	Move_7();	break;
	}

	const bool& IS_HIT = IPlayer::HitCheckCircl(hitRange, pos);
	if (IS_HIT)	Damage(1);
}


void Enemy::Move_0()
{
	const bool& IS_IN = (elapsedTime >= 0 && elapsedTime <= param.stop_time);
	const bool& IS_OUT = (elapsedTime >= param.out_time);
	const double& BRAKE = 0.03;

	// 最初の移動
	if (IS_IN)
	{
		// プレイヤーの前か画面の上半分なら
		if (IPlayer::GetPos().y - 60.0 > pos.y || pos.y >= 320.)
		{
			// 減速
			vSpeed.y = (IPlayer::GetPos().y - 60. - pos.y) * BRAKE;
		}

		// 降りる
		if (isMove)	pos.y += vSpeed.y;

		// 横移動：左から右
		if (pos.x < IPlayer::GetPos().x - 10. && isMove)
		{
			vSpeed.x = (IPlayer::GetPos().x - 20. - pos.x) * BRAKE;
			pos.x += vSpeed.x;
		}

		// 横移動２：右から左
		if (pos.x > IPlayer::GetPos().x + 10. && isMove)
		{
			pos.x -= vSpeed.x;
			vSpeed.x *= 0.99999999;
		}

		// 速度を負にしない
		if (vSpeed.x < 0.)
		{
			vSpeed.x = 0.;
			isMove = false;
		}
		if (vSpeed.y < 0.)	vSpeed.y = 0.;

	}

	// 自機に向く
	if (elapsedTime < param.out_time)
		AngleTarget(IPlayer::GetPos().x, IPlayer::GetPos().y);

	// スピード変更
	if (elapsedTime == param.out_time)
		vSpeed.y = 0.;

	// 帰る（笑）
	if (elapsedTime >= param.out_time)
	{
		// 加速
		if (vSpeed.y > -SPEED_0)
			vSpeed.y -= 0.05;

		// 移動
		pos.y += vSpeed.y;

		// 方向転換のつもり（納得いかない動きなので修正する計画）
		if (angle >= 0.)	angle += 0.02;
		if (angle < 0.)		angle -= 0.02;
	}

	// 退場
	if (IS_OUT)
	{
		if (pos.y <= -40)
		{
			isExist = false;
			IEnemyMng::CountDownEneNum();
		}
	}
}


void Enemy::Move_1()
{
	vSpeed.x = std::cos(elapsedTime / 30.) * 8. * std::sin(elapsedTime / 10.) * std::cos(elapsedTime / 10.);

	pos += vSpeed;

	// 自機に向く
	AngleTarget(IPlayer::GetPos().x, IPlayer::GetPos().y);

	if (pos.y > 490.)
	{
		isExist = false;
		IEnemyMng::CountDownEneNum();
	}
}


void Enemy::Move_2()
{
	vSpeed.SetVec(0., 2.);

	pos += vSpeed;

	if (pos.y > 490.)
	{
		isExist = false;
		IEnemyMng::CountDownEneNum();
	}
}


void Enemy::Move_3()
{
}


void Enemy::Move_4()
{
	static float c_move = 0.f;
	c_move += 0.01f;

	vSpeed.SetVec(0.7, 0.2);

	if (s_time < param.stop_time ||
		s_time > param.stop_time + 40)
	{
		pos.y += vSpeed.y;
	}

	pos.x += vSpeed.x * std::cos(c_move);

	if (pos.y > 490.)
	{
		isExist = false;
		IEnemyMng::CountDownEneNum();
	}
}


void Enemy::Fire()
{
	switch (param.type)
	{
	case 0: Fire_0();	break;
	case 1: Fire_1();	break;
	case 2:	Fire_2();	break;
	case 3:	Fire_3();	break;
	case 4:	Fire_4();	break;
	case 5:	Fire_5();	break;
	case 6:	Fire_6();	break;
	case 7:	Fire_7();	break;
	}
}


void Enemy::Fire_0()
{
	const double& ANGLE = atan2(IPlayer::GetPos().y - pos.y, IPlayer::GetPos().x - pos.x);

	if (elapsedTime == 20)
	{
		int dir = GetRand(1);
		double addAng = (GetRand(3) / 15.);
		if (dir == 0)
			IEneShot::Fire_Ang(eShotType::normal, pos, 0, param.s_speed, ANGLE - addAng, 1, 0);
		else
			IEneShot::Fire_Ang(eShotType::normal, pos, 0, param.s_speed, ANGLE + addAng, 1, 0);
	}

	if (elapsedTime == 50)
	{
		int dir = GetRand(1);
		double addAng = (GetRand(3) / 15.);
		if (dir == 0)
			IEneShot::Fire_Ang(eShotType::normal, pos, 0, param.s_speed, ANGLE - addAng, 1, 0);
		else
			IEneShot::Fire_Ang(eShotType::normal, pos, 0, param.s_speed, ANGLE + addAng, 1, 0);
	}

	// 3 ways shot
	if (elapsedTime == param.out_time && isUngry)
	{
		int dir = GetRand(1);
		double addAng = (GetRand(3) / 15);
		if (dir == 0)
		{
			IEneShot::Fire_Ang(eShotType::normal, pos, 0, param.s_speed, (ANGLE + 0.3) - addAng, 1, 0);
			IEneShot::Fire_Ang(eShotType::normal, pos, 0, param.s_speed,  ANGLE - addAng,        1, 0);
			IEneShot::Fire_Ang(eShotType::normal, pos, 0, param.s_speed, (ANGLE - 0.3) - addAng, 1, 0);
		}
		else
		{
			IEneShot::Fire_Ang(eShotType::normal, pos, 0, param.s_speed, (ANGLE + 0.3) - addAng, 1, 0);
			IEneShot::Fire_Ang(eShotType::normal, pos, 0, param.s_speed, ANGLE - addAng, 1, 0);
			IEneShot::Fire_Ang(eShotType::normal, pos, 0, param.s_speed, (ANGLE - 0.3) - addAng, 1, 0);
		}
	}
}


void Enemy::Fire_1()
{
	const Vector2D& dir = Vector2D::GetVec2(pos, IPlayer::GetPos());
	Vector2D force = dir.Normalize() * param.s_speed;

	if (elapsedTime >= param.stop_time)
	{
		if (elapsedTime == param.stop_time + 10 ||
			elapsedTime == param.stop_time + 20)
			IEneShot::Fire(eShotType::normal, pos, 0, force, 0, 0);
	}
}


void Enemy::Fire_2()
{
}


void Enemy::Fire_3()
{
}


void Enemy::Fire_4()
{
	/* snipe shot */
	if (s_time == param.stop_time - 10)
		vangle = atan2(IPlayer::GetPos().y - pos.y, IPlayer::GetPos().x - pos.x);

	/* 4WAY x 2 */
	if (s_time == param.stop_time ||
		s_time == param.stop_time + 20 ||
		s_time == param.stop_time + 40)
	{
		/* right side */
		IEneShot::Fire_Ang(eShotType::star, Vector2D::GetVec(pos.x + 70, pos.y - 30), -0.2, param.s_speed, DX_PI / 8., 1, 0);
		IEneShot::Fire_Ang(eShotType::star, Vector2D::GetVec(pos.x + 70, pos.y - 30), -0.2, param.s_speed, DX_PI / 6., 1, 0);
		IEneShot::Fire_Ang(eShotType::star, Vector2D::GetVec(pos.x + 50, pos.y), -0.2,      param.s_speed, DX_PI / 4., 1, 0);
		IEneShot::Fire_Ang(eShotType::star, Vector2D::GetVec(pos.x + 50, pos.y), -0.2,      param.s_speed, DX_PI / 3., 1, 0);

		/* left side */
		IEneShot::Fire_Ang(eShotType::star, Vector2D::GetVec(pos.x - 70, pos.y - 30), -0.2, param.s_speed, DX_PI * 0.85, 1, 0);
		IEneShot::Fire_Ang(eShotType::star, Vector2D::GetVec(pos.x - 70, pos.y - 30), -0.2, param.s_speed, DX_PI * 0.8,  1, 0);
		IEneShot::Fire_Ang(eShotType::star, Vector2D::GetVec(pos.x - 50, pos.y), -0.2,      param.s_speed, DX_PI * 0.7,  1, 0);
		IEneShot::Fire_Ang(eShotType::star, Vector2D::GetVec(pos.x - 50, pos.y), -0.2,      param.s_speed, DX_PI * 0.6,  1, 0);
	}

	/* snipe shot */
	if (s_time == param.stop_time + 20 ||
		s_time == param.stop_time + 30 ||
		s_time == param.stop_time + 40 ||
		s_time == param.stop_time + 50)
	{
		IEneShot::Fire_Ang(eShotType::wave, pos, 0, param.s_speed + 2, vangle, 1, 0);
	}

	/* reset shot_time */
	if (s_time == param.stop_time + 160)
		s_time = 0;
}


void Enemy::Fire_5()
{
}


void Enemy::Fire_6()
{
}


void Enemy::Fire_7()
{
}



void Enemy::Damage(int damage)
{
	switch (param.type)
	{
	case 0:
		param.hp -= damage;
		isDamage = true;
		IScore::AddScore(1);

		// 元気ならここで返す
		if (param.hp > 0)	return;

		ItemDrop();
		isExist = false;
		IScore::AddScore(400);
		Effector::PlayAnime(pos.x, pos.y, eExplosion_small);
		IEnemyMng::CountDownEneNum();
		break;
	case 1:
		break;
	case 2:
		param.hp -= damage;
		isDamage = true;
		IScore::AddScore(1);

		// 元気ならここで返す
		if (param.hp > 0)	return;

		ItemDrop();

		isExist = false;

		IScore::AddScore(900);
		Effector::PlayAnime(pos.x, pos.y, eExplosion_small);
		Effector::PlaySpread(pos.x, pos.y, GetRand(100), eSpread_SmallAll);
		IEnemyMng::CountDownEneNum();
		break;

	case 3:
		break;

	case 4:
		param.hp -= damage;
		isDamage = true;
		IScore::AddScore(1);

		// 元気ならここで返す
		if (param.hp > 0)	return;

		isExist = false;
		
		PlaySoundMem(sh_voice, DX_PLAYTYPE_BACK);
		
		IScore::AddScore(1500);
		Effector::PlayAnime(pos.x, pos.y, eExplosion_normal);
		Effector::PlaySpread(pos.x, pos.y, GetRand(100), eSpread_Bigs);
		Effector::PlaySpread(pos.x, pos.y, GetRand(100), eSpread_SmallBlue);
		IEnemyMng::CountDownEneNum();

		if (pos.y < 0)
		{
			IItemMng::Create(Vector2D(pos.x, pos.y + 60), eItem::Bomber);
			IItemMng::Create(Vector2D(pos.x, pos.y + 60), eItem::Power);
			IItemMng::Create(Vector2D(pos.x, pos.y + 60), eItem::Shield);
		}
		else {
			IItemMng::Create(Vector2D(pos.x, pos.y + 20), eItem::Bomber);
			IItemMng::Create(Vector2D(pos.x, pos.y + 20), eItem::Power);
			IItemMng::Create(Vector2D(pos.x, pos.y + 20), eItem::Shield);
		}
		break;

	case 5:
		break;

	case 6:
		break;

	case 7:
		break;

	default: assert(!"out of range");
	}

	if (param.hp <= 0)
		IStage::Quake(eQuake::normal);
}


void Enemy::ItemDrop()
{
	int rate = 100;
	if (GetRand(rate--) < DROP_RATE)	IItemMng::Create(pos, eItem::Power);
}