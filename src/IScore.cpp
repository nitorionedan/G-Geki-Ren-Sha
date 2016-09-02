#include "IScore.hpp"


std::shared_ptr<Score> IScore::mScore;


void IScore::set(std::shared_ptr<Score> score)
{
	mScore = score;
}


void IScore::AddScore(int point)
{
	mScore->AddScore(point);
}
