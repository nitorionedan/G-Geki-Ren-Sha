#pragma once
#include <memory>
#include "Counter.hpp"


/*
	”CˆÓ‚Ì“Á’è‚ÌŠÔ‚Ìˆ—ƒNƒ‰ƒX
	–¼‚Ã‚¯‚É¢‚éI
*/
class TimeValue
{
private:
	std::unique_ptr<Counter> c_time;

private:
	TimeValue();

public:
	TimeValue(int time);
	~TimeValue();
	void GetTimeValue(double* value, double time, double delta);
};