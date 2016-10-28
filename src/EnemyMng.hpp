#ifndef ENEMY_CSV_HPP
#define ENEMY_CSV_HPP

#include "Enemys.hpp"
#include "Stage.hpp"
#include "BossShotMgr.hpp"
#include "BossChara.hpp"
#include <vector>
#include <string>


class EnemyMng
{
public:
	EnemyMng();
	~EnemyMng();

	// @brief	そのステージの敵のデータをロード
	void Load(eStage stage);
	void Update(const Player& player);
	void Draw();
	void Finalize();
	void CountDownEneNum();
	void BossStart(eStage stage);
	void OffPacific() {
		f_pacifist = false;
	}
	bool IsHit(const double& ColX, const double& ColY, const int& DAMAGE);
	bool IsHit(const int& ColCircle, const double& ColX, const double& ColY, const int& Damage);
	const bool IsBossZone() const { // Is here boss zone?
		return isBossZone;
	}
	bool CheckPacific() const {
		return f_pacifist;
	}
	int GetEneNum() const {
		return enemyCount;
	}

private:
	void GetAllEnemyNum(int* enemyNum, std::string fileName);

	//std::vector<std::shared_ptr<Enemy>> enemy;				// 雑魚敵
	std::vector<Enemys*> enemy;									// 雑魚敵
	std::shared_ptr<BossShotMgr> bossShot;
	int enemyCount;							// 生き残っている敵の数
	bool isBossZone;
	bool f_pacifist;
};


class IEnemyMng
{
public:
	~IEnemyMng() {}
	static void set(std::shared_ptr<EnemyMng>);
	static void reset() {
		mEnemyMng.reset();
	}
	static void Load(eStage);
	static void CountDownEneNum();
	static 	void OffPacific() {
		mEnemyMng->OffPacific();
	}
	static bool IsHit(const double& ColX, const double& ColY, const int& DAMAGE);
	static bool IsHit(const int& ColCircle, const double& ColX, const double& ColY, const int& Damage);
	static const bool IsBossZone() {
		return mEnemyMng->IsBossZone();
	}
	static bool CheckPacifist() {
		return mEnemyMng->CheckPacific();
	}
	static int GetEneNum() {
		return mEnemyMng->GetEneNum();
	}

private:
	IEnemyMng() { mEnemyMng = nullptr; }
	static std::shared_ptr<EnemyMng> mEnemyMng;
};


#endif