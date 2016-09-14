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
}


Score::~Score()
{
	SaveScore();
}


void Score::Initialize()
{
	score = 0;

	std::ifstream ifs(RankingFile);
	int col = 0;

	if(ifs.is_open())
	{
		std::string buf;
		
		while (!ifs.eof())
		{
			char tmpChar = ifs.get();
			
			if (tmpChar == EOF)
				break;

			if(tmpChar == '\n')
			{
				++col;
				buf.clear();
				continue;
			}

			if (tmpChar == ',')
			{
				switch (col)
				{
				case 0:
					tRanking.date[col].Year = std::stoi(buf.c_str());
					break;
				case 1:
					tRanking.date[col].Mon = std::stoi(buf.c_str());
					break;
				case 2:
					tRanking.date[col].Day = std::stoi(buf.c_str());
					break;
				case 3:
					tRanking.score[col] = std::stoi(buf.c_str());
					break;
				case 4:
					tRanking.name[col] = std::stoi(buf.c_str());
					break;
				}
				buf.clear();
			}
			buf += tmpChar;
		}
	}
	else // file open failed
	{
		ifs.close();
		
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

		std::ofstream ofs(RankingFile);
		
		for (int i = 0; i < RankingNum; i++)
		{
			ofs << "year=" << tRanking.date[i].Year << ",";
			ofs << "month=" << tRanking.date[i].Mon << ",";
			ofs << "day=" << tRanking.date[i].Day << ",";
			ofs << "score=" << tRanking.score[i] << ",";
			ofs << "name=" << tRanking.name[i] << ",\n";
		}

		ofs.close();
	}



	/* TEST */
	for (int i = 0; i < RankingNum; ++i)
		printfDx("%d, name = %s\n", i, tRanking.name[i]);

	for(auto& name : tRanking.name)
		name = "YADEN";
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

	/* ranking save */
	std::ofstream ofs(RankingFile);
	for (int i = 0; i < RankingNum; i++)
	{
		ofs << "year=" << tRanking.date[i].Year << ",";
		ofs << "month=" << tRanking.date[i].Mon << ",";
		ofs << "day=" << tRanking.date[i].Day << ",";
		ofs << "score=" << tRanking.score[i] << ",";
		ofs << "name=" << tRanking.name[i] << ",\n";
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


void Score::DeleteScore()
{
	score = 0;
	hi_score = 0;
}