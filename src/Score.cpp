#include <DxLib.h>
#include "Score.hpp"
#include "Keyboard.hpp"
#include "DebugMode.hpp"
#include "IScore.hpp"
#include <fstream>
#include <cassert>
// この心は１つで　１つのようで無限で～

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
	LoadRanking();	// ランキングデータをロードするよ
	
	Initialize();
}


Score::~Score()
{
	SaveScore();
	SaveRanking();
}


void Score::Initialize()
{
	score = 0;
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
}


void Score::IsShowScore(const bool& f_IsShow){
	f_exist = f_IsShow;
}


void Score::AddScore(const int& point){
	score += point;
	tRanking[0].score += point;
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
}

void Score::SaveRanking()
{
	std::ofstream ofs(RankingFile, std::ios::binary);
	for (auto ranking : tRanking)
	{
		ofs << "score=" << ranking.score     << ",";
		ofs << "name="  << ranking.name      << ",";
		ofs << "year="  << ranking.date.Year << ",";
		ofs << "month=" << ranking.date.Mon  << ",";
		ofs << "day="   << ranking.date.Day  << ",";
		ofs << "\n";
	}
	ofs.close();
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


void Score::LoadRanking()
{
	/* secret */
	for (auto& ranking : tRanking)
	{
		ranking.score = 4444;
		ranking.name = "O MA E";
		GetDateTime(&ranking.date);
	}

	/* file read */
	int hFile = FileRead_open(RankingFile, FALSE);

	if (hFile == 0) // if file open failed
	{
		/* init ranking */
		tRanking[0].score = 111111;
		tRanking[1].score = 111110;
		tRanking[2].score = 111100;
		tRanking[3].score = 111000;
		tRanking[4].score = 110000;
		tRanking[5].score = 100000;
		tRanking[0].name = "YOSSIN";
		tRanking[1].name = "ZUN";
		tRanking[2].name = "DIXQ";
		tRanking[3].name = "ENDLESS SHIRAFU";
		tRanking[4].name = "JIRURUN";
		tRanking[5].name = "YADEN";
		for (auto& ranking : tRanking)
			GetDateTime(&ranking.date);
	}
	else // successful
	{
		std::string buf;
		int col = 0;

		while (1)
		{
			char tmpChar = FileRead_getc(hFile);

			if (tmpChar == EOF)	break;
			
			if (tmpChar == '\n')
			{
				++col;
				buf.clear();
				continue;
			}

			buf += tmpChar;

			if (tmpChar == '=')
			{
				if (buf == "score=")
				{
					buf.clear();

					while(1)
					{
						tmpChar = FileRead_getc(hFile);
						
						if (tmpChar == ',')
							break;

						buf += tmpChar;
					}

					tRanking[col].score = std::stoi(buf);
				}
				else if (buf == "name=")
				{
					buf.clear();

					while (1)
					{
						tmpChar = FileRead_getc(hFile);

						if (tmpChar == ',')
							break;

						buf += tmpChar;
					}

					tRanking[col].name = buf;
				}
				else if (buf == "year=")
				{
					buf.clear();

					while (1)
					{
						tmpChar = FileRead_getc(hFile);

						if (tmpChar == ',')
							break;

						buf += tmpChar;
					}

					tRanking[col].date.Year = std::stoi(buf);
				}
				else if (buf == "month=")
				{
					buf.clear();

					while (1)
					{
						tmpChar = FileRead_getc(hFile);

						if (tmpChar == ',')
							break;

						buf += tmpChar;
					}

					tRanking[col].date.Mon = std::stoi(buf);
				}
				else if (buf == "day=")
				{
					buf.clear();

					while (1)
					{
						tmpChar = FileRead_getc(hFile);

						if (tmpChar == ',')
							break;

						buf += tmpChar;
					}
					tRanking[col].date.Day = std::stoi(buf);
				}
				buf.clear();

			}
		}
	}

	FileRead_close(hFile);

	//for (int i = 0; i < RankingNum; ++i)
	//	printfDx("%d : score = %d\n", i, tRanking[i].score);
}


void Score::DeleteScore()
{
	score = 0;
	hi_score = 0;
}