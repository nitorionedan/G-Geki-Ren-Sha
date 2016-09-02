#include "EnemyMng.hpp"
#include "DebugMode.hpp"
#include "NullEnemyShot.hpp"

#include <DxLib.h>
#include <cassert>
#include <fstream>
#include <string>
#include <algorithm>


EnemyMng::EnemyMng()
	: bossShot(new BossShotMgr(new NullEnemyShot))
	, mBoss(nullptr)
{
	enemy.reserve(100);

	enemyCount = 0;
	isBossZone = false;
}


EnemyMng::~EnemyMng()
{
	Finalize();
}


void EnemyMng::Load(eStage stage)
{
	assert(stage == eStage::stage1 && "[EnemyMng::Load]　Sorry, I haven't been implement this stage XD\n");

	// File name: Enemys table.
	std::string fname;

	if (!enemy.empty())
		Finalize();

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
	enemyCount = 0;
	std::ifstream ifs_count(fname, std::ios::in);
	assert(ifs_count.is_open() && "EnemyMng::Load()");
	
	while (!ifs_count.eof())
	{
		char tmp = ifs_count.get();
		if (tmp == '\n')
			enemyCount++;
	}
	enemyCount--;	// adjust enemy num

	printfDx("%d\n", enemyCount);

	ifs_count.close();

	/* Create enemys data */
	tEnemyData* ene_date;
	ene_date = new tEnemyData[enemyCount];

	std::string buf;
	int nowCol = 1;
	int nowRow = 0;
	std::ifstream ifs(fname);

	while (ifs.get() != '\n'); // header skip

	while(1)
	{
		char tmpChar;
		while(1)
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
	for (int i = 0; i < enemyCount; i++)
	{
		enemy.emplace_back( new Enemy(
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
			ene_date[i].item) );
	}

	delete[] ene_date;
}


void EnemyMng::Update(const Player& player)
{
	bossShot->Update(player);

	for (auto i : enemy)
		i->Update(player);
}


void EnemyMng::Draw()
{
	bossShot->Draw();

	for (auto i : enemy)
		i->Draw();

	if(DebugMode::isTest)
		DrawFormatString(200, 200, GetColor(0, 255, 0), "敵の数：%d", enemyCount);
}


void EnemyMng::Finalize()
{
	for (auto i : enemy)
		delete i;
	enemy.erase(std::begin(enemy), std::end(enemy));
	enemy.clear();
}


void EnemyMng::setup(std::shared_ptr<BossChara> boss)
{
	mBoss = boss;
}


void EnemyMng::CountDownEneNum()
{
	enemyCount--;

	if (enemyCount == 0)
		BossStart(eStage::stage1);
}


void EnemyMng::BossStart(eStage stage)
{
	switch (stage)
	{
	case eStage::stage1:
		mBoss->Start(eBoss_A);
		bossShot->ChangeShot(eEnemyShot::ShotA);
		break;

	case eStage::stage2:
		break;
	
	case eStage::stage3:
		break;
	
	case eStage::stage4:
		break;
	
	case eStage::stage5:
		break;
	
	case eStage::stage6:
		break;
	
	case eStage::stage0:
		break;
	default:	assert("EnemyMng::BossStart()");
	}

}


bool EnemyMng::IsHit(const double & ColX, const double & ColY, const int& DAMAGE)
{
	bool isHit;

	for(auto i : enemy)
	{
		isHit = i->IsHit(ColX, ColY, DAMAGE);
		if (isHit)	return true;
	}

	// ここまで来たということは当たっていないということ
	return false;
}


bool EnemyMng::IsHit(const int & ColCircle, const double & ColX, const double & ColY, const int & Damage)
{
	bool isHit;

	for (auto i : enemy)
	{
		isHit = i->IsHit(ColCircle, ColX, ColY, Damage);
		if (isHit)	return true;
	}

	// ここまで来たということは当たっていないということ
	return false;
}


/********************************************
コメント　：	csvファイルの読み込みに関して
参考サイト：	http://bituse.info/game/shot/11
アクセス日：	2016/5/13

*********************************************/



/*
@brief	敵管理インターフェイス
*/
std::shared_ptr<EnemyMng> IEnemyMng::mEnemyMng;


void IEnemyMng::set(std::shared_ptr<EnemyMng> enemyMng)
{
	mEnemyMng = enemyMng;
}


void IEnemyMng::Load(eStage stage)
{
	mEnemyMng->Load(stage);
}


void IEnemyMng::CountDownEneNum()
{
	mEnemyMng->CountDownEneNum();
}


bool IEnemyMng::IsHit(const double & ColX, const double & ColY, const int & DAMAGE)
{
	return mEnemyMng->IsHit(ColX, ColY, DAMAGE);
}


bool IEnemyMng::IsHit(const int & ColCircle, const double & ColX, const double & ColY, const int & Damage)
{
	return mEnemyMng->IsHit(ColCircle, ColX, ColY, Damage);
}
