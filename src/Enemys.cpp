#include "Enemys.hpp"
#include "ZakoEnemy.hpp"
#include <cassert>


Enemys::Enemys(const tEnemyData & param)
	: m_enemy(nullptr)
{	
	switch (param.type)
	{
	case 0: 
		m_enemy = static_cast<ZakoEnemy*>(new ZakoEnemy_Ze(param));
		break;
	case 2:
		m_enemy = static_cast<ZakoEnemy*>(new ZakoEnemy_Den(param));
		break;
	case 4:
		m_enemy = static_cast<ZakoEnemy*>(new ZakoEnemy_Career(param));
		break;
	default: assert( !"out of rage" );
	}
}


Enemys::~Enemys()
{
	delete m_enemy;
}


void Enemys::Update()
{
	m_enemy->StartCheck();
	if ( !m_enemy->IsExist() )
		return;
	m_enemy->UpdateTime();
	m_enemy->ResetState();
	m_enemy->UngryCheck();
	m_enemy->Update();
}


void Enemys::Draw()
{
	if ( !m_enemy->IsExist() )
		return;

	/* Flushing */
	if (m_enemy->IsWeak() && m_enemy->GetElapsedTime() % 12 >= 10)
	{
		SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255);
		SetDrawBright(255, 0, 0);
	}

	/* Damage */
	if (m_enemy->IsHit())
		SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255);

	m_enemy->Draw();

	/* Reset normal render */
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetDrawBright(255, 255, 255);
}

bool Enemys::IsHit(const double& Range, const double& ColX, const double& ColY, const double& DAMAGE) const {
	return m_enemy->HitCheck(Range, ColX, ColY, DAMAGE);
}

bool Enemys::IsHit(const double X, const double Y, const double Damage) {
	return m_enemy->HitCheck(X, Y, Damage);
}
