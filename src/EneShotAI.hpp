#pragma once
#include "EneShot.hpp"

class EneShotAI
{
public:
	virtual ~EneShotAI() {}
	virtual void Update(tShot& shot) = 0;
};

class EneShotAI_Straight : public EneShotAI
{
public:
	virtual void Update(tShot& shot) override;
};

class EneShotAI_OutsideCurve : public EneShotAI
{
public:
	virtual void Update(tShot& shot) override {}
};

class EneShotAI_InsideCurve : public EneShotAI
{
public:
	virtual void Update(tShot& shot) override {}
};

class EneShotAI_Wave : public EneShotAI
{
public:
	virtual void Update(tShot& shot) override {}
};