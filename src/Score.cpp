#include <DxLib.h>
#include "Score.hpp"
#include "Keyboard.hpp"
#include "DebugMode.hpp"
#include "IScore.hpp"
#include <fstream>
#include <cassert>

const int Score::digit = 8;


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
	
	Initialize();

	printfDx("Score()\n");
}


Score::~Score()
{
	SaveScore();
	printfDx("~Score()\n");
}


void Score::Initialize()
{
	score = 0;

	/* read ranking file */
	HANDLE hFile = CreateFile("data/rkg.txt", GENERIC_ALL, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	/* failed */
	if (hFile == INVALID_HANDLE_VALUE)
	{
		assert(!"????");

		/* init ranking */
		tRanking.score[0] = 111111;
		tRanking.score[1] = 111110;
		tRanking.score[2] = 111100;
		tRanking.score[3] = 111000;
		tRanking.score[4] = 110000;
		tRanking.score[5] = 100000;
		tRanking.name[0] = "YOSSIN";
		tRanking.name[1] = "ZUN";
		tRanking.name[2] = "DIXQ";
		tRanking.name[3] = "JIRURUN";
		tRanking.name[4] = "ENDLESS SHIRAFU";
		tRanking.name[5] = "YADEN";
		for (int i = 0; i < _countof(tRanking.date); ++i)
			GetDateTime(&tRanking.date[i]);

		/* write */
		t_Ranking* lp_tRanking = &tRanking;
		DWORD dwWritten;
		WriteFile(hFile, (LPCVOID)lp_tRanking, sizeof(tRanking), &dwWritten, NULL);
	}
	else
	{
		/* read */
		t_Ranking* lp_tRanking = &tRanking;
		DWORD dwRead;
		ReadFile(hFile, (LPVOID)lp_tRanking, sizeof(tRanking), &dwRead, NULL);
	}

	CloseHandle(hFile);

	for (int i = 0; i < 6; ++i)
		printfDx("%d, name = %s, score = %d\n", i, tRanking.name[i].c_str(), tRanking.score[i]);

	tRanking.name[0] = "YADEN";

	for (int i = 0; i < 6; ++i)
		printfDx("%d, name = %s, score = %d\n", i, tRanking.name[i].c_str(), tRanking.score[i]);
}


void Score::Update()
{
	if (Keyboard::Instance()->isPush(KEY_INPUT_Y))	clsDx();
	if (val_score != score)				val_score = score;
	if (val_hiscore != hi_score)		val_hiscore = hi_score;
	if (score > hi_score)				hi_score = score;

	// TEST
	if (!DebugMode::isTest)	return;


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
	/* high score */
	if ((error = fopen_s(&fp, name, "wb")) != 0)
		printfDx("ファイルオープンエラー\n再起動してね。(^^)/~~\n");
	else
	{
		fwrite(&hi_score, sizeof(hi_score), 1, fp);
		fclose(fp);
	}

	/* read ranking file */
	HANDLE hFile = CreateFile("data/rkg.txt", GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwWritten;
	t_Ranking* lp_tRanking = &tRanking;
	WriteFile(hFile, (LPCVOID)lp_tRanking, sizeof(t_Ranking), &dwWritten, NULL);
	CloseHandle(hFile);
}


void Score::ShowResult()
{

}


void Score::LoadScore()
{
	// もし開けなければ
	if ((error = fopen_s(&fp, name, "rb")) != 0)
	{
		// 新しく作ってデータを保存
		SaveScore();
		printfDx("\n新しく[score_data.dat]を作成しました。\n[Y] キー：おｋ\n");
	}
	// 開ければ
	else
	{
		// データをロード
		fread(&hi_score, sizeof(hi_score), 1, fp);
		fclose(fp);
	}
}


void Score::DeleteScore()
{
	score = 0;
	hi_score = 0;
}