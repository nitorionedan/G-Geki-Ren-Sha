#pragma once
#include "EneShotMove.hpp"


class EneShotStraight : public EneShotMove
{
public:
	virtual void Move(EneShotCreater::EneShot* pEShot) override;
};