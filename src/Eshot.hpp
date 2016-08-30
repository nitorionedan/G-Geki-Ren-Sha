#pragma once

#include "Bullet.hpp"
#include "Counter.hpp"
#include "EnemyShot.hpp"
#include "Effect.hpp"
#include <array>
#include <memory>


enum class eAttackState
{
	Atk1,
	Atk2,
};	// 攻撃の種類


/// BossAから攻撃開始からの経過時間や、残りHPの情報を得て切り替えていく
class Eshot : public EnemyShot
{
public:
	Eshot();
	~Eshot();
	virtual void Update(const Player& player) override;
	virtual void Draw() override;
	void SetAttackState(eAttackState state) {
		atkState = state;
	}
	void SetFirePos();
	void Fire();

	void Move(const Player& player);

	static void GoFire1() {
		s_isFire1 = true;
	}

	eAttackState atkState;										// 攻撃状態

private:
	static const double SC_LIMIT_XL;							// 画面内ボーダー
	static const double SC_LIMIT_XR;
	static const double SC_LIMIT_YT;
	static const double SC_LIMIT_YB;
	static double const HIT_RANGE;

	const int		A_INTERVAL;									// ショットＡの攻撃間隔
	const int		A_FIRE_NUM;									// ショットＡの発射数
	const double	A_SPEED;									// ～の速さ
	const double	A_BASE_ANGLE;								// ～の基準となる角度

	void Reset() {}												// 弾が何かに当たったときに呼ばれる
	bool isOverLimit(const double X_POS, const double Y_POS);	// 画面外へ出たか？
	bool IsHit(const double & otherRange, const Vector2D other);

	std::unique_ptr<Effect> effect;
	std::unique_ptr<Counter> c_atk1, c_atk2;					// 次の攻撃をするまでの待機時間
	std::unique_ptr<Counter> c_fire1, c_fire2;					// 攻撃中のカウント（弾の間隔を空ける用）
	std::array<Bullet*, 64>	shotA;								// 16方向へ飛ばす弾

	int hg_shotA, hg_shotB;										// 画像用ハンドル
	int hg_fireA, hg_fireB;										// 発射したときに光るやつのアレ（説明できん！）
	int hs_shotA;												// 音用ハンドル
	int loopCount;
	double speed;
	std::array<double, 2>	x_fireA, y_fireA;
	
	static bool s_isFire1, s_isFire2;
};