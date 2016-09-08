#include "EneShotStraight.hpp"
#include "DxLib.h"


void EneShotStraight::Move(EneShotCreater::EneShot* pEShot)
{
	pEShot->pos.y += pEShot->speed;
}