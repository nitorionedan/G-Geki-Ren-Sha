/*
@brief	ŽG‹›“G‚Ì’e
*/
#pragma once

#include "Player.hpp"
#include "Vector2D.hpp"
#include <memory>


class EnemyShot
{
public:
	EnemyShot(){}
	virtual ~EnemyShot(){}
	virtual void Update(const Player& player) = 0;
	virtual void Draw() = 0;
};