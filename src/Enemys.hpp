#pragma once

#include <memory>

class ZakoEnemy;
class ZakoEnemy_Ze;
class ZakoEnemy_Den;
class ZakoEnemy_Career;


struct tEnemyData
{
	int type,		// �G���
		stype,		// �e���
		m_pattern,	// �ړ��p�^�[��
		s_pattern,	// ���˃p�^�[��
		in_time,	// �o������
		stop_time,	// ��~����
		shot_time,	// �e���ˎ���
		out_time,	// �A�Ҏ���
		x_pos,		// x���W
		y_pos,		// y���W
		s_speed,	// �e�X�s�[�h
		hp,			// HP
		item;		// �A�C�e��
};


class Enemys
{
public:
	explicit Enemys(const tEnemyData& param);
	~Enemys();
	void Update();
	void Draw();
	void UngryCheck();
	void HitCheckToPlayer();
	bool IsHit(const double& Range, const double& ColX, const double& ColY, const double& DAMAGE) const;
	bool IsHit(const double X, const double Y, const double Damage);

private:
	Enemys() {}
	ZakoEnemy* m_enemy;
};