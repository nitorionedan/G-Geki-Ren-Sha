#pragma once
#include "Score.hpp"
#include <memory>


class IScore
{
public:
	IScore() {}
	~IScore() {}
	static void set(std::shared_ptr<Score> score);
	static void reset() {
		mScore.reset();
	}
	static void AddScore(int point);

private:
	static std::shared_ptr<Score> mScore;
};