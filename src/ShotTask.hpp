#pragma once
#include "Player.hpp"


class ShotTask
{
public:
	ShotTask(){}
	virtual ~ShotTask(){}

	virtual void Update(const double& PosX, const double& PosY) = 0;
	virtual void Draw() = 0;

	// @brief					���ˁi1���j
	// @param[in]	vec_speed	�i�ޑ��x
	// @param[in]	vec_angle	�i�ފp�x
	virtual void Fire(const double& SPEED, const double& ANGLE) = 0;
	virtual void Fire(const double& PosX, const double& PosY, const double& SPEED, const double& ANGLE) = 0;

	// @brief					�����蔻��
	// @param[in]	ColX, ColY	�Փ˂��镨�̂̍��W
	// @param[in]	ColR		�Փ˂��镨�̂̓�����ό`
	// @out						�����������ǂ���
	virtual bool IsHit(const double & ColX, const double & ColY, const double & ColR) = 0;
};