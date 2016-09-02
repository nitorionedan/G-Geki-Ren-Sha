#include "IScore.hpp"
#include <cassert>

std::shared_ptr<Score> IScore::mScore;


void IScore::set(std::shared_ptr<Score> score)
{
	mScore = score;
	assert(mScore != nullptr && "IScore::set()");
}


void IScore::AddScore(int point)
{
	mScore->AddScore(point);
}
