#include <DxLib.h>
#include "Score.hpp"
#include "Keyboard.hpp"
#include "DebugMode.hpp"
#include "IScore.hpp"
#include <fstream>

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
	LoadScore();	// �n�C�X�R�A�f�[�^�����[�h�����
	
	Initialize();

	printfDx("Score()\n");
}


Score::~Score()
{
	SaveScore();
	FILE* rankingFile;
	fopen_s(&rankingFile, "data/ranking.txt", "wb");
	fwrite(&tRanking, sizeof(tRanking), 1, rankingFile);
	fclose(rankingFile);
	printfDx("~Score()\n");
}


void Score::Initialize()
{
	score = 0;

	/* crete files */
	FILE* rankingFile;
	if ( error = fopen_s(&rankingFile, "data/ranking.txt", "wb") != 0 )
	{
		printfDx("file open error\n�ċN�����ĂˁB(^^)/~~\n");

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
		fwrite(&tRanking, sizeof(tRanking), 1, rankingFile);
	}
	else // successful
	{
		printfDx("successful\n");

		// �f�[�^�����[�h
		fread(&tRanking, sizeof(tRanking), 1, fp);
	}

	fclose(rankingFile);

	for (int i = 0; i < 6; ++i)
		printfDx("%d, name = %s, score = %d\n", i, tRanking.name[i].c_str(), tRanking.score[i]);

	tRanking.name[0] = "YADEN";
}


void Score::Update()
{
	if(Keyboard::Instance()->isPush(KEY_INPUT_Y))	clsDx();
	if (val_score != score)				val_score = score;
	if (val_hiscore != hi_score)		val_hiscore = hi_score;
	if (score > hi_score)				hi_score = score;

	// TEST
	if (!DebugMode::isTest)	return;


//	ShowResult();
}


void Score::Draw()
{
	if (!f_exist)	return; // �\�����Ȃ�

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
		printfDx("�t�@�C���I�[�v���G���[\n�ċN�����ĂˁB(^^)/~~\n");
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
	// �����J���Ȃ����
	if ((error = fopen_s(&fp, name, "rb")) != 0)
	{
		// �V��������ăf�[�^��ۑ�
		SaveScore();
		printfDx("\n�V����[score_data.dat]���쐬���܂����B\n[Y] �L�[�F����\n");
	}
	// �J�����
	else
	{
		// �f�[�^�����[�h
		fread(&hi_score, sizeof(hi_score), 1, fp);
		fclose(fp);
	}
}


void Score::DeleteScore()
{
	score = 0;
	hi_score = 0;
}