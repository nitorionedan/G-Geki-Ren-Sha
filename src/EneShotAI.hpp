#pragma once
#include "EneShot.hpp"
#include "Vector2D.hpp"


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
	EneShotAI_OutsideCurve();
	virtual void Update(tShot& shot) override;

private:
	double dis;     // distance
	double count, variationCount;
	double rad, addRad, len;
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


// @referenced	http://www7.plala.or.jp/kfb/program/stg2dvec.html
class EneShotAI_Homing : public EneShotAI
{
public:
	EneShotAI_Homing();
	virtual void Update(tShot& shot) override;

private:
	int time;
	double addSin, addCos;
	bool canHoming;
};