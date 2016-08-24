#include <DxLib.h>

#include "EnemyMng.hpp"
#include "Game.hpp"
#include "DebugMode.hpp"

#include <cstdlib>
#include <cstring>
#include <cassert>


Enemy** EnemyMng::enemy;
int     EnemyMng::e_num;
int		EnemyMng::ene_count;
bool	EnemyMng::isBossZone;


EnemyMng::EnemyMng()
	: T_T(1)
{
	ene_count = 0;
	isBossZone = false;

	// 空の敵を作成
	enemy = new Enemy*[2];
	for (int i = 0; i < 2; i++)	enemy[i] = nullptr;
}


EnemyMng::~EnemyMng()
{
	for (int i = 0; i < sizeof(enemy) / sizeof(enemy[0]); i++)	delete enemy[i];
	delete[]	enemy;
}


void EnemyMng::Load(eStage stage)
{
	assert(stage == eStage::stage1 && "[EnemyMng::Load]　Sorry, I haven't been implement this stage XD\n");

	e_num = 0;

	// File name: Enemys table.
	char* fname = nullptr;

	if (enemy != nullptr)
	{
		for (int i = 0; i < sizeof(enemy) / sizeof(enemy[0]); i++)	delete enemy[i];
		delete[]	enemy;
	}

	// Load enemys table.
	switch (stage)
	{
	case eStage::stage1: fname = "resource/EnemyTable1.csv"; break;
	case eStage::stage2: fname = "resource/EnemyTable2.csv"; break;
	case eStage::stage3: fname = "resource/EnemyTable3.csv"; break;
	case eStage::stage4: fname = "resource/EnemyTable4.csv"; break;
	case eStage::stage5: fname = "resource/EnemyTable5.csv"; break;
	case eStage::stage6: fname = "resource/EnemyTable6.csv"; break;
	case eStage::stage0: fname = "resource/EnemyTable0.csv"; break;
	default:	assert("EnemyMng::Load() : over of range");
	}

	FILE* fp;
	int c;

	fopen_s(&fp, fname, "rb");

	// Count enemys num.
	{
		while (T_T)
		{
			c = std::fgetc(fp);

			if (c == EOF)	break;		// 末尾ならループを抜ける

			if (c == '\n')	e_num++;	// 改行なら、カウント
		}

		e_num--; // 数合わせ

		fclose(fp);
	}

	// Set enemys num.
	enemy = new Enemy*[e_num];
	ene_count = e_num;

	// Create enemys data.
	tEnemyData* ene_date;
	ene_date = new tEnemyData[e_num];

	char buf[100];
	int col = 1;
	int row = 0;

	std::memset(buf, 0, sizeof(buf));
	fopen_s(&fp, fname, "rb");

	// Header skip.
	while (std::fgetc(fp) != '\n');

	while (T_T)		// 涙
	{

		while (T_T)	// そして涙
		{
			c = std::fgetc(fp);

			// 末尾ならループを抜ける
			if (c == EOF)	goto out;

			// カンマか改行以外なら、文字として連結
			if (c != ',' && c != '\n'){
				strcat_s(buf, sizeof(buf) / sizeof(buf[0]), (const char*)&c);
			}
			// カンマか改行ならループを抜ける
			else {
				break;
			}

		}

		// ここに来たということは、1セル文の文字列が出来上がったといこうこと
		switch (col)
		{
					// 1列目は敵種類を表す。atoi関数で数値として代入
		case 1:		ene_date[row].type = std::atoi(buf);		break;
					// 2列目は弾種類。以降省略
		case 2:		ene_date[row].stype = std::atoi(buf);		break;
		case 3:		ene_date[row].m_pattern = std::atoi(buf);	break;
		case 4:		ene_date[row].s_pattern = std::atoi(buf);	break;
		case 5:		ene_date[row].in_time = std::atoi(buf);		break;
		case 6:		ene_date[row].stop_time = std::atoi(buf);	break;
		case 7:		ene_date[row].shot_time = std::atoi(buf);	break;
		case 8:		ene_date[row].out_time = std::atoi(buf);	break;
		case 9:		ene_date[row].x_pos = std::atoi(buf);		break;
		case 10:	ene_date[row].y_pos = std::atoi(buf);		break;
		case 11:	ene_date[row].s_speed = std::atoi(buf);		break;
		case 12:	ene_date[row].hp = std::atoi(buf);			break;
		case 13:	ene_date[row].item = std::atoi(buf);		break;
		}

		// バッファを初期化
		std::memset(buf, 0, sizeof(buf));

		// 列数を足す
		++col;

		// もし読み込んだ文字列が改行だったら列数を初期化して行数を増やす
		if (c == '\n')
		{
			col = 1;
			++row;
		}
	}

out:
	// ファイルを閉じる
	std::fclose(fp);

	// 敵オブジェクト生成
	for (int i = 0; i < e_num; i++)
	{
		enemy[i] = new Enemy(
			ene_date[i].type,
			ene_date[i].stype,
			ene_date[i].m_pattern,
			ene_date[i].s_pattern,
			ene_date[i].in_time,
			ene_date[i].stop_time,
			ene_date[i].shot_time,
			ene_date[i].out_time,
			ene_date[i].x_pos,
			ene_date[i].y_pos,
			ene_date[i].s_speed,
			ene_date[i].hp,
			ene_date[i].item);
	}

	// 用は無くなったため解放
	delete[] ene_date;
}


void EnemyMng::Update()
{
	if (enemy == nullptr)	return;

	for (int i = 0; i < e_num; i++)	enemy[i]->Update();
}


void EnemyMng::Draw()
{
	if (enemy == nullptr)	return;

	for (int i = 0; i < e_num; i++)	enemy[i]->Draw();

	// デバッグ中なら
	if(DebugMode::isTest)
		DrawFormatString(200, 200, GetColor(0, 255, 0), "敵の数：%d", ene_count);
}


void EnemyMng::CountDownEneNum()
{
	ene_count--;

	if (ene_count == 0)	Game::StartBoss();
}


bool EnemyMng::IsHit(const double & ColX, const double & ColY, const int& DAMAGE)
{
	if (enemy == nullptr)	return false;

	bool isHit;

	for (int i = 0; i < e_num; i++)
	{
		isHit = enemy[i]->IsHit(ColX, ColY, DAMAGE);

		// 当ていいていればここで返す
		if (isHit)	return isHit;
	}

	// ここまで来たということは当たっていないということ
	return isHit;
}


bool EnemyMng::IsHit(const int & ColCircle, const double & ColX, const double & ColY, const int & Damage)
{
	if (enemy == nullptr)	return false;

	bool isHit;

	for (int i = 0; i < e_num; i++)
	{
		isHit = enemy[i]->IsHit(ColCircle, ColX, ColY, Damage);

		// 当ていいていればここで返す
		if (isHit)	return isHit;
	}

	// ここまで来たということは当たっていないということ
	return isHit;
}


/********************************************
コメント　：	csvファイルの読み込みに関して
参考サイト：	http://bituse.info/game/shot/11
アクセス日：	2016/5/13

*********************************************/

// EOF