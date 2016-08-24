#pragma once

#include "ShotTask.hpp"
#include "Vector2D.hpp"
#include "Effect.hpp"

#include <array>
#include <memory>


class EShot03 : public ShotTask
{
public:
	EShot03();
	~EShot03();
	virtual void Update(const double& PosX, const double& PosY) override;
	virtual void Draw() override;

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

	const int HIT_RANGE;		// 当たり範囲
	const int ALL_FRAME_NUM;	// アニメーションの総枚数
	const int FRAME_TIME;		// １コマの表示時間（フレーム数）
	static const int ALL_NUM = 30;

	int gh[4];
	std::unique_ptr<Effect> effect;
	std::array<int, ALL_NUM> time;			// 経過時間
	std::array<Vector2D, ALL_NUM> pos;		// 座標
	std::array<double, ALL_NUM> vspeed;		// 弾の速さ
	std::array<double, ALL_NUM> vangle;		// 進む角度
	std::array<double, ALL_NUM> ang;		// 進む角度
	std::array<double, ALL_NUM> rad;		// 画像の回転値
	std::array<double, ALL_NUM> rota;		// 増加回転値
	std::array<double, ALL_NUM> isExist;	// 存在しているか？
};

// EOF