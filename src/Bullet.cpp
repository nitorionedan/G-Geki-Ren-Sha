#include "Dxlib.h"

#include "Bullet.hpp"


Bullet::Bullet()
	: atk(0)
	, x_pos(0.0)
	, y_pos(0.0)
	, width(0)
	, height(0)
	, angle(0.0)
	, speed(0.0)
	, f_exist(false)
{
}


Bullet::~Bullet() {

}


void Bullet::Set(const double X_POS, const double Y_POS)
{
	x_pos = X_POS;
	y_pos = Y_POS;
}