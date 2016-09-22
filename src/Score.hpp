#pragma once

#include "Graphic.hpp"
#include <memory>
#include <DxLib.h>
#include <string>

constexpr int RankingNum = 6;
constexpr char* RankingFile = "data/rkg.dat";


class Score
{
	struct t_Ranking
	{
		int score;
		std::string name;
		DATEDATA date;
	};

public:
	Score();
	~Score();
	void Initialize();
	void Update();
	void Draw();
	void IsShowScore(const bool& IsShow);
	void ShowResult();
	void AddScore(const int& point);
	int secBonus; // 毎秒加算スコア // なしの方向で(^^;
	/*const t_Ranking& GetRanking() {
		return tRanking;
	}*/

private:
	void LoadScore();				// セーブファイルロード
	void LoadRanking();
	void SaveScore();
	void SaveRanking();
	void DeleteScore();				// セーブデータ削除

	std::unique_ptr<Graphic> graphic;
	char* name;						// セーブファイル
	FILE* fp;						// ファイルアクセスポインタ
	errno_t error;					// 例外

	static const int digit;			// 桁数
	int score;
	int x_pos, y_pos;
	int hi_score, x_hi, y_hi;
	int val_score;
	int val_hiscore;
	int x_num;
	int fileHandle;
	int c_secscore;	// 毎秒加算スコア用カウンター
	bool f_exist;	// 存在しているか？

	t_Ranking tRanking[RankingNum];
};