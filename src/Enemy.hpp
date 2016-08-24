#pragma once

#include "Task.hpp"
#include "Counter.hpp"
#include "ShotTask.hpp"
#include "Shot.hpp"
#include "Vector2D.hpp"
#include "ShotTask.hpp"

#include <memory>


class Enemy : public Task
{
public:
	// @param[in]	type	“Gí—Şi‚O`‚R‚Oj
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
	void Update()	override;
	void Draw()		override;
	bool IsHit(const double& ColX, const double& ColY, const int& DAMAGE);
	bool IsHit(const int & ColCircle, const double & ColX, const double & ColY, const int & Damage);

	// ƒ^[ƒQƒbƒg•ûŒü‚ÖŒü‚­
	void AngleTarget(double Target_x, double Target_y);

private:
	void Move();
	void Move_0();
	void Move_1();
	void Move_2();
	void Move_3();
	void Move_4();
	void Move_5();
	void Move_6();
	void Move_7();
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

	// “G‰æ‘œƒnƒ“ƒhƒ‹
	int gh_ene00;		// G‹›‚P(ze)
	int gh_ene01;		// G‹›‚Q(kam)
	int gh_ene02[4];	// G‹›‚R(raide)
	int gh_ene03;		// G‹›‚Sasa
	int gh_ene04[8];	// G‹›‚Scareer

	// ”­‰Î‰æ‘œƒnƒ“ƒhƒ‹
	int gh_shot00;

	// ’f–––‚
	int sh_voice;

	// “Gî•ñ
	int elapsedTime;	// Œo‰ßŠÔ
	int moveTime;		// “®‚­ŠÔ
	int s_time;
	double angle;		// Œü‚«
	double vangle;
	double vspeed_x;	// ‚˜•ûŒü‚ÌƒxƒNƒgƒ‹
	double vspeed_y;	// ‚™•ûŒü
	double hitRange;	// “–‚½‚è”ÍˆÍ
	Vector2D hitSRange;
	Vector2D pos;		// À•W
	bool isExist;		// ‘¶İ‚µ‚Ä‚¢‚é‚©H
	bool isMove;		// “®‚¢‚Ä‚¢‚é‚©H
	bool isDamage;		// ƒ_ƒ[ƒW‚ğó‚¯‚½‚©H
	bool isUngry;		// ƒvƒŒƒCƒ„[‚ªUŒ‚‚µ‚½‚©H

	// ’e
	Shot* shot;
	Shot* shot2;
	Shot* shot3;

	// \‘¢‘Ìƒf[ƒ^‚É‡‚í‚¹‚é
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

// EOF