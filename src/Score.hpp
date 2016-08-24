#pragma once

#include "Task.hpp"
#include "Graphic.hpp"

#include <memory>


class Score : public Task
{
public:
	Score();
	~Score();
	void Update() override;
	void Draw() override;
	void IsShowScore(const bool& IsShow);
	void ShowResult();
	static void AddScore(const int& point);

	int secBonus; // 毎秒加算スコア // なしの方向で(^^;

private:
	void LoadScore();				// セーブファイルロード
	void SaveScore();
	void DeleteScore();				// セーブデータ削除

	std::unique_ptr<Graphic> graphic;
	char* name;						// セーブファイル
	FILE* fp;						// ファイルアクセスポインタ
	errno_t error;					// 例外

	static const int digit;			// 桁数
	static int score;
	int x_pos, y_pos;
	int hi_score, x_hi, y_hi;
	int val_score;
	int val_hiscore;
	int x_num;

	int c_secscore;					// 毎秒加算スコア用カウンター
	bool f_exist;					// 存在しているか？
};