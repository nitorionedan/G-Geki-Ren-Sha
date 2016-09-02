#ifndef ENEMY_CSV_HPP
#define ENEMY_CSV_HPP

#include "Enemy.hpp"
#include "Stage.hpp"
#include "BossShotMgr.hpp"
#include "BossChara.hpp"
#include <vector>


struct tEnemyData
{
	int type,		// 敵種類
		stype,		// 弾種類
		m_pattern,	// 移動パターン
		s_pattern,	// 発射パターン
		in_time,	// 出現時間
		stop_time,	// 停止時間
		shot_time,	// 弾発射時間
		out_time,	// 帰還時間
		x_pos,		// x座標
		y_pos,		// y座標
		s_speed,	// 弾スピード
		hp,			// HP
		item;		// アイテム
};


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
	void setup(std::shared_ptr<BossChara>);
	void CountDownEneNum();
	void BossStart(eStage stage);
	bool IsHit(const double& ColX, const double& ColY, const int& DAMAGE);
	bool IsHit(const int& ColCircle, const double& ColX, const double& ColY, const int& Damage);

private:
	//std::vector<std::shared_ptr<Enemy>> enemy;				// 雑魚敵
	std::vector<Enemy*> enemy;									// 雑魚敵

	std::shared_ptr<BossShotMgr> bossShot;
	std::shared_ptr<BossChara> mBoss;
	int enemyCount;							// 生き残っている敵の数
	bool isBossZone;						// 敵の数
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