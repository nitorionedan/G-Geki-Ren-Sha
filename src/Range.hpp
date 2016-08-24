#pragma once

#include "DxLib.h"

#undef max
#undef min

template<typename T>
class Range
{
public:
	Range(){}

	Range(T min, T max) : min_(min), max_(max){
		assert(min_ <= max_);
	}

	// 範囲内か？
	bool isInside(T value) const{
		return (min_ <= value) && (value <= max_);
	}

/*
	// 範囲内にクランプする
	T clamp(T value) const{
		return ::clamp(value, min_, max_); // 外部の関数
	}

	// 範囲内でラップアラウンドする
	T wrap(T value) const{
		return ::wrap(value, min_, max_); // 外部の関数
	}
*/

	// 下限値の取得
	T min() const{
		return min_;
	}

	// 上限値の取得
	T max() const{
		return max_;
	}

private:
	T min_; // 下限値
	T max_; // 上限値
};