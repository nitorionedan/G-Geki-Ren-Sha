#include <DxLib.h>
#include "EnemyMng.hpp"
#include "Game.hpp"
#include "DebugMode.hpp"
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <fstream>
#include <string>


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
	for (int i = 0; i < sizeof(enemy) / sizeof(enemy[0]); i++)
		delete enemy[i];
	delete[]	enemy;
}


void EnemyMng::Load(eStage stage)
{
	assert(stage == eStage::stage1 && "[EnemyMng::Load]　Sorry, I haven't been implement this stage XD\n");

	// File name: Enemys table.
	std::string fname;

	if (enemy != nullptr)
	{
		for (int i = 0; i < sizeof(enemy) / sizeof(enemy[0]); i++)
			delete enemy[i];
		delete[]	enemy;
	}

	// Load enemys table.
	switch (stage)
	{
	case eStage::stage1: fname = "data/EnemyTable1.csv"; break;
	case eStage::stage2: fname = "data/EnemyTable2.csv"; break;
	case eStage::stage3: fname = "data/EnemyTable3.csv"; break;
	case eStage::stage4: fname = "data/EnemyTable4.csv"; break;
	case eStage::stage5: fname = "data/EnemyTable5.csv"; break;
	case eStage::stage6: fname = "data/EnemyTable6.csv"; break;
	case eStage::stage0: fname = "data/EnemyTable0.csv"; break;
	default:	assert("EnemyMng::Load() : over of range");
	}

	/* count enemys num */
	e_num = 0;
	std::ifstream ifs_count(fname, std::ios::in);
	assert(ifs_count.is_open() && "EnemyMng::Load()");
	
	while (!ifs_count.eof())
	{
		char tmp = ifs_count.get();
		if (tmp == '\n')
			e_num++;
	}
	
	e_num--;	// adjust enemy num
	ifs_count.close();

	/* set enemys num */
	enemy = new Enemy*[e_num];
	ene_count = e_num;

	/* Create enemys data */
	tEnemyData* ene_date;
	ene_date = new tEnemyData[e_num];

	std::string buf;
	int nowCol = 1;
	int nowRow = 0;
	std::ifstream ifs(fname);

	while (ifs.get() != '\n'); // header skip

	while(T_T)
	{
		char tmpChar;
		while(T_T)
		{
			tmpChar = ifs.get();
			
			if (tmpChar == EOF)
				goto out;
			
			if (tmpChar != ',' && tmpChar != '\n')
				buf += tmpChar;
			else
				break;
		}

		switch (nowCol)
		{
		// num of column is one which is enemy type
		case 1:		ene_date[nowRow].type = std::stoi(buf);			break;
		// omitted below
		case 2:		ene_date[nowRow].stype = std::stoi(buf);		break;
		case 3:		ene_date[nowRow].m_pattern = std::stoi(buf);	break;
		case 4:		ene_date[nowRow].s_pattern = std::stoi(buf);	break;
		case 5:		ene_date[nowRow].in_time = std::stoi(buf);		break;
		case 6:		ene_date[nowRow].stop_time = std::stoi(buf);	break;
		case 7:		ene_date[nowRow].shot_time = std::stoi(buf);	break;
		case 8:		ene_date[nowRow].out_time = std::stoi(buf);		break;
		case 9:		ene_date[nowRow].x_pos = std::stoi(buf);		break;
		case 10:	ene_date[nowRow].y_pos = std::stoi(buf);		break;
		case 11:	ene_date[nowRow].s_speed = std::stoi(buf);		break;
		case 12:	ene_date[nowRow].hp = std::stoi(buf);			break;
		case 13:	ene_date[nowRow].item = std::stoi(buf);			break;
		}

		buf.clear();
		nowCol++;

		// seek
		if(tmpChar == '\n')
		{
			nowCol = 1;
			nowRow++;
		}
	}

out:
	ifs.close();
	buf.clear();

	// create enemys
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

	delete[] ene_date;
}


void EnemyMng::Update()
{
	if (enemy == nullptr)	return;

	for (int i = 0; i < e_num; i++)
		enemy[i]->Update();
}


void EnemyMng::Draw()
{
	if (enemy == nullptr)	return;

	for (int i = 0; i < e_num; i++)
		enemy[i]->Draw();

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

		if (isHit)	return isHit;
	}

	// ここまで来たということは当たっていないということ
	return false;
}


bool EnemyMng::IsHit(const int & ColCircle, const double & ColX, const double & ColY, const int & Damage)
{
	if (enemy == nullptr)
		return false;

	bool isHit;

	for (int i = 0; i < e_num; i++)
	{
		isHit = enemy[i]->IsHit(ColCircle, ColX, ColY, Damage);

		if (isHit)	return isHit;
	}

	// ここまで来たということは当たっていないということ
	return false;
}


/********************************************
コメント　：	csvファイルの読み込みに関して
参考サイト：	http://bituse.info/game/shot/11
アクセス日：	2016/5/13

*********************************************/

// EOF