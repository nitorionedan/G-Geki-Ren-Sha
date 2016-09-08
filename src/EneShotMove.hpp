#pragma once

#include "EneShotCreater.hpp"


class EneShotMove
{
public:
	virtual void Move(EneShotCreater::EneShot* pEShot) = 0;
};