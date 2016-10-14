#include "EneShotAI.hpp"
#include <DxLib.h>


void EneShotAI_Straight::Update(tShot & shot)
{
	shot.pos     += shot.force;
	shot.force.x *= shot.accel;
	shot.force.y *= shot.accel;
}

void EneShotAI_OutsideCurve::Update(tShot & shot)
{
	shot.pos     += shot.force;
	shot.force.x *= shot.accel;
	shot.force.y *= shot.accel;
}