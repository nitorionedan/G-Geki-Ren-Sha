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
	int secBonus; // ���b���Z�X�R�A // �Ȃ��̕�����(^^;
	/*const t_Ranking& GetRanking() {
		return tRanking;
	}*/

private:
	void LoadScore();				// �Z�[�u�t�@�C�����[�h
	void LoadRanking();
	void SaveScore();
	void SaveRanking();
	void DeleteScore();				// �Z�[�u�f�[�^�폜

	std::unique_ptr<Graphic> graphic;
	char* name;						// �Z�[�u�t�@�C��
	FILE* fp;						// �t�@�C���A�N�Z�X�|�C���^
	errno_t error;					// ��O

	static const int digit;			// ����
	int score;
	int x_pos, y_pos;
	int hi_score, x_hi, y_hi;
	int val_score;
	int val_hiscore;
	int x_num;
	int fileHandle;
	int c_secscore;	// ���b���Z�X�R�A�p�J�E���^�[
	bool f_exist;	// ���݂��Ă��邩�H

	t_Ranking tRanking[RankingNum];
};