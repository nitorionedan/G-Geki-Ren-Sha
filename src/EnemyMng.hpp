#ifndef ENEMY_CSV_HPP
#define ENEMY_CSV_HPP

#include "Enemy.hpp"
#include "Stage.hpp"


typedef struct
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
} tEnemyData;


class EnemyMng
{
public:
	EnemyMng();
	~EnemyMng();

	// @brief	そのステージの敵のデータをロード
	void Load(eStage stage);
	void Update();
	void Draw();
	static void CountDownEneNum();
	static bool IsHit(const double& ColX, const double& ColY, const int& DAMAGE);
	static bool IsHit(const int& ColCircle, const double& ColX, const double& ColY, const int& Damage);

private:
	// ループ用
	const int T_T;

	// 雑魚敵
	static Enemy** enemy;

	// 敵の数
	static int e_num;

	static int ene_count;

	static bool isBossZone;
};


#endif