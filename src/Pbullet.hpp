#pragma once

#include "Task.hpp"
#include "Bullet.hpp"
#include "Counter.hpp"
#include "Keyboard.hpp"
#include "Player.hpp"
#include <memory>
#include <array>


class Pbullet									// 今思えば「shot」のほうがしっくりくるし、短い。俺のアホー！
{
public:
	Pbullet();
	~Pbullet();
	void Update();
	void Draw();
	void SetFirePos(const double* x, const double* y);
	void SetPlayerPow(int powlv);
	void FireEffect(double fx, double fy);

private:
	void Fire();									// 発射フラグをたてる
	void Move();									// フラグが立っている弾を動かす
	void Reset(int num);							// 画面外の弾をリセット

	const int AllBulletNum;							// 最大弾数
	const double SPEED;								// 弾の速さ
	const double Angle;								// 飛ばす方向(角度)
	const double LimScXR, LimScXL;					// 弾の生存範囲
	const double LimScYT, LimScYB;					//
	std::array<Bullet*, 4> Abullet;					// 青
	std::array<Bullet*, 4> Bbullet;					// 黄
	std::array<Bullet*, 4> Cbullet;					// 赤
	std::array<Bullet*, 4> Dbullet;					// 青の隣（角度が変わる弾）
	std::unique_ptr<Counter> c_blt;					// ショット間隔用カウンター
	std::unique_ptr<Counter> c_fire;				// 発射直後の発行画像用カウンター

	int		hg_blue, hg_orange, hg_red, hg_eftest;	// 画像用ハンドル
	int		shiftLevel;								// ショットレベル
	double	x_pos, y_pos;
	bool	isShot;									// 撃ったか		// 発音が伊豆諸島だｗｗｗｗｗｗｗｗｗｗ
};
// EOF