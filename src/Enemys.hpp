#pragma once

#include <memory>

class ZakoEnemy;
class ZakoEnemy_Ze;
class ZakoEnemy_Den;
class ZakoEnemy_Career;


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