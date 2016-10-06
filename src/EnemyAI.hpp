#pragma once
#include "Vector2D.hpp"


class EnemyAI
{
public:
	explicit EnemyAI(Vector2D pos) {
		this->pos = pos;
	}
	EnemyAI(){}
	virtual ~EnemyAI() {}
	virtual void Update() = 0;
	const Vector2D& GetPos() const {
		return pos;
	}

protected:
	Vector2D pos;
};