#pragma once

#include "ShotTask.hpp"
#include "Vector2D.hpp"

#include <array>


class EShot02: public ShotTask
{
public:
	EShot02::EShot02(int colorNum);
	~EShot02();
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
	EShot02();

	// @param[in]	id	添え字番号
	void Move(const int& id);

	const int HIT_RANGE;		// 当たり範囲
	static const int ALL_NUM = 30;

	int gh;

	std::array<Vector2D, ALL_NUM> pos;		// 座標
	std::array<int, ALL_NUM> time;			// 経過時間
	std::array<double, ALL_NUM> vspeed;		// 弾の速さ
	std::array<double, ALL_NUM> vangle;		// 進む角度
	std::array<double, ALL_NUM> rad;		// 画像の回転値
	std::array<double, ALL_NUM> rota;		// 増加回転値
	std::array<double, ALL_NUM> isExist;	// 存在しているか？
};