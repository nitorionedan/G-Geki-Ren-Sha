#pragma once
#include "Stage.hpp"


class StageData
{
private:
	char*   name = "stage_data.dat"; // どのステージから始めるか...
	FILE*   fp;
	errno_t error;

public:
	StageData();
	~StageData();

	void SetFirstStage(eStage estage); // 最初に始めるステージを設定configで設定されたものから盗ってくる
	void LoadStage(); // コンティニューするとき使いまふ
	void SaveStage(); // コンティニューするとき使いまふ
};