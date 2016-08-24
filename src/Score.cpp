#include "DxLib.h"

#include "Score.hpp"
#include "Keyboard.hpp"
#include "DebugMode.hpp"

#include <fstream>


const int Score::digit = 8;

int Score::score;


Score::Score()
	: graphic(new Graphic)
	, name("score_data.dat")
	, x_pos(200)
	, y_pos(10)
	, x_hi(600)
	, y_hi(y_pos)
	, hi_score(0)
	, val_score(0)
	, c_secscore(0)
	, secBonus(0)
	, f_exist(true)
{
	LoadScore();	// ハイスコアデータをロードするよ
	score = 0;
}


Score::~Score()
{
	SaveScore();
}


void Score::Update()
{
	if(Keyboard_Get(KEY_INPUT_Y) == 1)	clsDx();
	if (val_score != score)				val_score = score;
	if (val_hiscore != hi_score)		val_hiscore = hi_score;
	if (score > hi_score)				hi_score = score;

	// TEST
	if (!DebugMode::isTest)	return;
	/*
	if (Keyboard_Get(KEY_INPUT_1) == 1)	AddScore(123); // test
	if (Keyboard_Get(KEY_INPUT_2) == 1)	DeleteScore(); // test
	*/

//	ShowResult();
}


void Score::Draw()
{
	if (!f_exist)	return; // 表示しない

	// Score
	graphic->DrawMyString2(x_pos - 160, y_pos, "SC", 16, true, 2.0);
	graphic->DrawScore(x_pos, y_pos, score, 15, 2.0);

	// HiScore
	graphic->DrawMyString2(x_hi - 160, y_hi, "HI", 16, true, 2.0);
	graphic->DrawScore(x_hi, y_hi, val_hiscore, 15, 2.0);

	// TEST
}


void Score::IsShowScore(const bool& f_IsShow){
	f_exist = f_IsShow;
}


void Score::AddScore(const int& point){
	score += point;
}


void Score::SaveScore()
{
	if ((error = fopen_s(&fp, name, "wb")) != 0)
		printfDx("ファイルオープンエラー\n再起動してね。(^^)/~~\n");
	else
	{
		fwrite(&hi_score, sizeof(hi_score), 1, fp);
		fclose(fp);
	}
}


void Score::ShowResult()
{

}


void Score::LoadScore()
{
	if ((error = fopen_s(&fp, name, "rb")) != 0)
	{
		SaveScore();
		printfDx("\n新しく[score_data.dat]を作成しました。\n[Y] キー：おｋ\n");
	}
	else
	{
		fread(&hi_score, sizeof(hi_score), 1, fp);
		fclose(fp);
	}
}


void Score::DeleteScore()
{
//	remove(name); // ファイルの削除なくてもええわ
	score = 0;
	hi_score = 0;
}