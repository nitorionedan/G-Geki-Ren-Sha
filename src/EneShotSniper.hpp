#pragma once

#include "EneShotMove.hpp"


class EneShotSniper : public EneShotMove
{
public:
	EneShotSniper(){}
	~EneShotSniper(){}
	virtual void Move(EneShotCreater::EneShot* pEShot) override;
};