#pragma once
#include <algorithm>

#undef max
#undef min

/*--------------------------------------

	Timerクラス
	機能：一定時間のある処理をさせる

--------------------------------------*/
class Timer
{
private:
	double nowTime_;   // 現在の時間
	double limitTime_; // 制限時間

public:
	Timer(double limitTime) : nowTime_(0), limitTime_(limitTime)
	{}

	void Update(double deltaTime){
		nowTime_ = std::min(nowTime_ + deltaTime, limitTime_);
	}

	void Reset() {
		nowTime_ = 0;
	}

	bool isTimeout() const {
		return nowTime_ >= limitTime_;
	}
};