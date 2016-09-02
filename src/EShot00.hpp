#pragma once

#include "ShotTask.hpp"
#include "Vector2D.hpp"
#include "Effect.hpp"

#include <array>
#include <memory>


class EShot00 : public ShotTask
{
public:
	EShot00();
	~EShot00();
	void Update(const double& PosX, const double& PosY) override;
	void Draw() override;

	// @brief					発射（1発）
	// @param[in]	vec_speed	進む速度
	// @param[in]	vec_angle	進む角度
	void Fire(const double& SPEED, const double& ANGLE) override;


	void Fire(const double& PosX, const double& PosY, const double& SPEED, const double& ANGLE) override;

	// @brief					当たり判定
	// @param[in]	ColX, ColY	衝突する物体の座標
	// @param[in]	ColR		衝突する物体の当たり変形
	// @out						当たったかどうか
	bool IsHit(const double& ColX, const double& ColY, const double& ColR) override;

private:
	// @param[in]	id	添え字番号
	void Move(const int& id);

	// @brief	アニメーション再生関数・改
//	void DrawAnime(const double& PosX, const double& PosY, const double& ANGLE, const int& TIME, const int& FRAME_NUM, const int& TIME_FRAME, int Gr_Handle[]);

	// @biref	時間を全て同じにするためのアニメーション再生関数
//	void DrawAnime(const double& PosX, const double& PosY, const double& ANGLE, const int& FRAME_NUM, const int& TIME_FRAME, int Gr_Handle[]);

	const int ALL_FRAME_NUM;	// アニメーションの総枚数
	const int FRAME_TIME;		// １コマの表示時間（フレーム数）
	const int ALL_FRAME_TIME;	// アニメーション１周の時間（ = ALL_FRAME_NUM * FRAME_TIME ）
	const int HIT_RANGE;		// 当たり範囲
	static const int ALL_NUM = 9;

	int gh[12];
	std::unique_ptr<Effect>	effect;
	std::array<int, ALL_NUM> time;			// 経過時間
	std::array<Vector2D, ALL_NUM> pos;		// 座標
	std::array<double, ALL_NUM> vspeed;		// 弾の速さ
	std::array<double, ALL_NUM> vangle;		// 進む角度
	std::array<double, ALL_NUM> rad;		// 画像の回転値
	std::array<double, ALL_NUM> rota;		// 増加回転値
	std::array<double, ALL_NUM> isExist;	// 存在しているか？
};

// EOF