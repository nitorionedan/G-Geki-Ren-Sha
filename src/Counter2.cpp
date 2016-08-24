#include "DxLib.h"

#include "Counter2.hpp"

#undef max
#undef min


Counter2::Counter2()
	: max(-1)
	, count(0)
{
}

void Counter2::SetMax(const int & MAX_)
{
	max = MAX_;
	count = max;
}

void Counter2::Update()
{
	count--;

	if (count == 0) count = max;
}


void Counter2::Reset(){
	count = max;
}


int Counter2::GetCount(){
	return count;
}


bool Counter2::isLast(){
	return (count == 0) ? true : false;
}

bool Counter2::isEach(const int & MIN_, const int & MAX_)
{
	if (MIN_ > MAX_) printfDx("”ÍˆÍƒGƒ‰[");

	return (count >= MIN_ && count <= MAX_) ? true : false;
}

// EOF