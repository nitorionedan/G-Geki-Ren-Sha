#ifndef ENEMY_CSV_HPP
#define ENEMY_CSV_HPP

#include "Enemy.hpp"
#include "Stage.hpp"
#include "BossShotMgr.hpp"
#include "BossChara.hpp"
#include <vector>


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


class EnemyMng
{
public:
	EnemyMng();
	~EnemyMng();

	// @brief	���̃X�e�[�W�̓G�̃f�[�^�����[�h
	void Load(eStage stage);
	void Update(const Player& player);
	void Draw();
	void Finalize();
	void setup(std::shared_ptr<BossChara>);
	void CountDownEneNum();
	void BossStart(eStage stage);
	bool IsHit(const double& ColX, const double& ColY, const int& DAMAGE);
	bool IsHit(const int& ColCircle, const double& ColX, const double& ColY, const int& Damage);

private:
	//std::vector<std::shared_ptr<Enemy>> enemy;				// �G���G
	std::vector<Enemy*> enemy;									// �G���G

	std::shared_ptr<BossShotMgr> bossShot;
	std::shared_ptr<BossChara> mBoss;
	int enemyCount;							// �����c���Ă���G�̐�
	bool isBossZone;						// �G�̐�
};


class IEnemyMng
{
public:
	~IEnemyMng() {}
	static void set(std::shared_ptr<EnemyMng>);
	static void Load(eStage);
	static void CountDownEneNum();
	static bool IsHit(const double& ColX, const double& ColY, const int& DAMAGE);
	static bool IsHit(const int& ColCircle, const double& ColX, const double& ColY, const int& Damage);

private:
	IEnemyMng() { mEnemyMng = nullptr; }
	static std::shared_ptr<EnemyMng> mEnemyMng;
};


#endif