#include "EneShotCreater.hpp"
#include <DxLib.h>
#include <cmath>
#include <algorithm>


EneShotCreater::EneShotCreater()
{
	LoadDivGraph("GRAPH/GAME/Eshot/eshot00.png", 12, 4, 3, 8, 8, gh_ball);
	LoadDivGraph("GRAPH/GAME/Eshot/eshot02.png", 24, 8, 3, 9, 33, gh_power);
	gh_long[0] = LoadGraph("GRAPH/GAME/Eshot/enaga0.png");
	gh_long[1] = LoadGraph("GRAPH/GAME/Eshot/enaga1.png");
	gh_long[2] = LoadGraph("GRAPH/GAME/Eshot/enaga2.png");
	gh_long[3] = LoadGraph("GRAPH/GAME/Eshot/enaga3.png");
	gh_long[4] = LoadGraph("GRAPH/GAME/Eshot/enaga4.png");
	LoadDivGraph("GRAPH/GAME/Eshot/bigshot.png", 4, 4, 1, 60, 60, gh_big);
	LoadDivGraph("GRAPH/GAME/Eshot/eshot04.png", 10, 5, 2, 13, 12, gh_suriken);

	Initialize();
}


EneShotCreater::~EneShotCreater()
{
	for (int i = 0; i < _countof(gh_ball); i++)
		DeleteGraph(gh_ball[i]);
	for (int i = 0; i < _countof(gh_power); i++)
		DeleteGraph(gh_power[i]);
	for (int i = 0; i < _countof(gh_long); i++)
		DeleteGraph(gh_long[i]);
	for (int i = 0; i < _countof(gh_big); i++)
		DeleteGraph(gh_big[i]);
	for (int i = 0; i < _countof(gh_suriken); i++)
		DeleteGraph(gh_suriken[i]);
}


void EneShotCreater::Update()
{
}


void EneShotCreater::Draw()
{
}


void EneShotCreater::Initialize()
{
}


void EneShotCreater::Move()
{
}


void EneShotCreater::CheckCollider()
{
}


void EneShotCreater::Fire(eEneShot shotType, eEneShotMove moveType, Vector2D& pos)
{
	
}

// EneShot(Inner class)==============================================================

EneShotCreater::EneShot::EneShot(eEneShot shotType, EneShotMove* moveType, Vector2D firstPos)
	: mMove(moveType)
{
	this->shotType = shotType;
	pos = firstPos;

	Initialize();
}
// TODO: anuahddsasd

EneShotCreater::EneShot::~EneShot()
{
	delete mMove;
}


void EneShotCreater::EneShot::Update(EneShotCreater * pEneShotCreater)
{
}


void EneShotCreater::EneShot::Draw(const int& GrHandle)
{
}


void EneShotCreater::EneShot::Initialize()
{
}