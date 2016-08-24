#pragma once


//
// カウンターをよく使うので作りました。
// 明らかに「Timer」っていう名前にのほうが良い。なぜなら減算形式だから。
//

class Counter
{
public:
	Counter();							// 普通のコンストラクタは使用禁止
	Counter::Counter(const int&  max);	// カウントする最大フレーム数を引数にする
	~Counter() {}
	void Update();						// これが呼ばれる度にカウントダウンします
	int  Remainder(const int  divisor);	// 引数と現在のカウントを割ったあまりを返します。
	int  GetNowcount();					// 現在のカウントを返します
	void isCount(const bool f_count_);	// カウントするかどうかを切り替える
	void Reset();						// 現在のカウンターを初期値に戻します
	bool isLast();						// 最後のカウントになったかどうか
	bool isEach(const int& MIN, const int& MAX); // 指定の範囲内ですか？

	const int  MAX_COUNT;				// 最大カウント

private:
	int  counter;						// カウンター
	bool f_count;						// カウントするかどうか
	bool f_wrap;						// ０になった後勝手にリセットするかどうか
};