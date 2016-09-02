#ifndef DEF_ENEMY_HPP
#define DEF_ENEMY_HPP

#include "Counter.hpp"
#include "Shot.hpp"
#include "Vector2D.hpp"
#include "Player.hpp"
#include <memory>


class Enemy
{
public:
	// @param[in]	type	敵種類（０〜３０）
	Enemy(
		int type,
		int stype,
		int m_pattern,
		int s_pattern,
		int in_time,
		int stop_time,
		int shot_time,
		int out_time,
		int x_pos,
		int y_pos,
		int s_speed,
		int hp,
		int item);
	~Enemy();
	void Update(const Player& player);
	void Draw();
	const bool IsExist() const {
		return isExist;
	}
	bool IsHit(const double& ColX, const double& ColY, const int& DAMAGE);
	bool IsHit(const int & ColCircle, const double & ColX, const double & ColY, const int & Damage);

	// ターゲット方向へ向く
	void AngleTarget(double Target_x, double Target_y);

private:
	void Move(const Player& player);
	void Move_0(const Player& player);
	void Move_1(const Player& player);
	void Move_2(const Player& player);
	void Move_3(const Player& player);
	void Move_4(const Player& player);
	void Move_5(const Player& player);
	void Move_6(const Player& player);
	void Move_7(const Player& player);
	void Move_8(const Player& player){}
	void Move_9(const Player& player){}
	void Fire(const Player& player);
	void Fire_0(const Player& player);
	void Fire_1(const Player& player);
	void Fire_2(const Player& player);
	void Fire_3(const Player& player);
	void Fire_4(const Player& player);
	void Fire_5(const Player& player);
	void Fire_6(const Player& player);
	void Fire_7(const Player& player);
	void Fire_8(const Player& player){}
	void Fire_9(const Player& player){}
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
	double vspeed_x;	// ｘ方向のベクトル
	double vspeed_y;	// ｙ方向
	double hitRange;	// 当たり範囲
	Vector2D hitSRange;
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
	int type;
	int stype;
	int m_pattern;
	int s_pattern;
	int in_time;
	int stop_time;
	int shot_time;
	int out_time;
	int x_pos;
	int y_pos;
	int s_speed;
	int hp;
	int item;
};

#endif