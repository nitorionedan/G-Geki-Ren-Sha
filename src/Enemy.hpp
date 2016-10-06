#ifndef DEF_ENEMY_HPP
#define DEF_ENEMY_HPP

#include "Counter.hpp"
#include "Shot.hpp"
#include "Vector2D.hpp"
#include "Player.hpp"
#include <memory>


enum class eEnemyParam
{
	type,
	stype,
	m_pattern,
	s_pattern,
	in_time,
	stop_time,
	shot_time,
	out_time,
	x_pos,
	y_pos,
	s_speed,
	hp,
	item,
};


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


class Enemy
{
public:
	// @param[in]	type	敵種類（０～３０）
	explicit Enemy(tEnemyData param);
	~Enemy();
	void Update();
	void Draw();
	const int GetParam(eEnemyParam param);
	const bool IsExist() const {
		return isExist;
	}
	bool IsHit(const double& ColX, const double& ColY, const int& DAMAGE);
	bool IsHit(const int & ColCircle, const double & ColX, const double & ColY, const int & Damage);

	// ターゲット方向へ向く
	void AngleTarget(double Target_x, double Target_y);

private:
	void Move();
	void Move_0();
	void Move_1();
	void Move_2();
	void Move_3();
	void Move_4();
	void Move_5(){}
	void Move_6(){}
	void Move_7(){}
	void Move_8(){}
	void Move_9(){}
	void Fire();
	void Fire_0();
	void Fire_1();
	void Fire_2();
	void Fire_3();
	void Fire_4();
	void Fire_5();
	void Fire_6();
	void Fire_7();
	void Fire_8(){}
	void Fire_9(){}
	void Damage(int damage);
	void ItemDrop();

	const int SCREEN_LIMIT_XL;
	const int SCREEN_LIMIT_XR;
	const int SCREEN_LIMIT_YT;
	const int SCREEN_LIMIT_YB;
	const int MAX_HP;
	const int DROP_RATE;

	// type_0
	const double SPEED_0;

	// 敵画像ハンドル
	int gh_ene00;		// 雑魚１(ze)
	int gh_ene01;		// 雑魚２(kam)
	int gh_ene02[4];	// 雑魚３(raide)
	int gh_ene03;		// 雑魚４asa
	int gh_ene04[8];	// 雑魚４career

	// 発火画像ハンドル
	int gh_shot00;

	// 断末魔
	int sh_voice;

	// 敵情報
	int elapsedTime;	// 経過時間
	int moveTime;		// 動く時間
	int s_time;
	double angle;		// 向き
	double vangle;
	double hitRange;	// 当たり範囲
	Vector2D hitSRange, vSpeed;
	Vector2D pos;		// 座標
	bool isExist;		// 存在しているか？
	bool isMove;		// 動いているか？
	bool isDamage;		// ダメージを受けたか？
	bool isUngry;		// プレイヤーが攻撃したか？

	// 弾
	Shot* shot;
	Shot* shot2;
	Shot* shot3;

	// 構造体データに合わせる
	tEnemyData param;
};

#endif