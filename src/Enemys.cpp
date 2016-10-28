#include "Enemys.hpp"
#include "ZakoEnemy.hpp"
#include "Keyboard.hpp"
#include "DebugMode.hpp"
#include <cassert>


Enemys::Enemys(const tEnemyData & param)
	: m_enemy(nullptr)
{	
	assert(param.stop_time < param.out_time);

	switch (param.type)
	{
	case 0: m_enemy = static_cast<ZakoEnemy*>(new ZakoEnemy_Ze(param)); break;
	case 1: m_enemy = static_cast<ZakoEnemy*>(new ZakoEnemy_Flower(param)); break;
	case 2:	m_enemy = static_cast<ZakoEnemy*>(new ZakoEnemy_Den(param)); break;
	case 4:	m_enemy = static_cast<ZakoEnemy*>(new ZakoEnemy_Career(param)); break;
	default: assert( !"out of rage" );
	}
}


Enemys::~Enemys()
{
	delete m_enemy;
}


void Enemys::Update()
{
	if (IEnemyMng::GetEneNum() == 0)
		return;

	m_enemy->StartCheck();
	if ( !m_enemy->IsExist() )
		return;
	m_enemy->UpdateTime();
	m_enemy->data.isHit = false; // reset state
	UngryCheck();
	m_enemy->Update();
	HitCheckToPlayer();
}


void Enemys::Draw()
{
	if (IEnemyMng::GetEneNum() == 0)
		return;

	if ( !m_enemy->IsExist() )
		return;

	/* Weaking EFCT */
	if (m_enemy->IsWeak() && m_enemy->GetElapsedTime() % 12 >= 10)
	{
		SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255);
		SetDrawBright(255, 0, 0);
	}

	/* Damage EFCT */
	if (m_enemy->IsHit())
		SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255);

	m_enemy->Draw();

	/* Reset normal render */
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetDrawBright(255, 255, 255);

	/* Debug */
	if (DebugMode::isTest)
	{
		DrawCircle(m_enemy->data.pos.x, m_enemy->data.pos.y, m_enemy->data.hitRange, GetColor(0, 0, 255), FALSE);
	}
}

void Enemys::UngryCheck()
{
	if (Keyboard::Instance()->isPush(KEY_INPUT_Z))
		m_enemy->data.isUngry = true;
}

void Enemys::HitCheckToPlayer()
{
	const bool& IS_HIT = IPlayer::HitCheckCircl(m_enemy->data.hitRange, m_enemy->data.pos);
	if (IS_HIT)	m_enemy->CalcDamage(1);
}

bool Enemys::IsHit(const double& Range, const double& ColX, const double& ColY, const double& DAMAGE) const {
	bool isHit = m_enemy->HitCheck(Range, ColX, ColY, DAMAGE);
	if (isHit)
		m_enemy->data.isHit = true;
	return isHit;
}

bool Enemys::IsHit(const double X, const double Y, const double Damage) {
	bool isHit = m_enemy->HitCheck(X, Y, Damage);
	if (isHit)
		m_enemy->data.isHit = true;
	return isHit;
}
