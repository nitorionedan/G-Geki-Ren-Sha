#pragma once
#include "Player.hpp"


class ShotTask
{
public:
	ShotTask(){}
	virtual ~ShotTask(){}

	virtual void Update(const double& PosX, const double& PosY) = 0;
	virtual void Draw() = 0;

	// @brief					発射（1発）
	// @param[in]	vec_speed	進む速度
	// @param[in]	vec_angle	進む角度
	virtual void Fire(const double& SPEED, const double& ANGLE) = 0;
	virtual void Fire(const double& PosX, const double& PosY, const double& SPEED, const double& ANGLE) = 0;

	// @brief					当たり判定
	// @param[in]	ColX, ColY	衝突する物体の座標
	// @param[in]	ColR		衝突する物体の当たり変形
	// @out						当たったかどうか
	virtual bool IsHit(const double & ColX, const double & ColY, const double & ColR) = 0;
};