#include "EneShotAI.hpp"


void EneShotAI_Straight::Update(tShot & shot)
{
	shot.pos     += shot.force;
	shot.force.x *= shot.accel;
	shot.force.y *= shot.accel;
}
