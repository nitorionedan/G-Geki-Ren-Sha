#include "DxLib.h"

#include "Title.hpp"
#include <algorithm>

#undef max
#undef min


Title::Title() : 
	Y_MIN(150.0)
{
	hg_title	= LoadGraph("GRAPH/MENU/title.png");
	hs_intro	= LoadSoundMem("SOUND/SE/intro.mp3");
	x			= 320.0;
	y			= 640.0;
	speed		= 6.3;		// 4.25
	isStop		= false;
}


Title::~Title()
{
	DeleteGraph(hg_title);
	DeleteSoundMem(hs_intro);
}

// タイトルライズイン
void Title::Update()
{
	if (isStop)	return;

	speed -= 0.04;									// ジョジョに奇妙に減速
	y = std::max(y, Y_MIN);							// ちょうどいい位置で止める
	if (y == Y_MIN)
	{
		PlaySoundMem(hs_intro, DX_PLAYTYPE_BACK);
		isStop = true;								// 目的の位置で停止
	}
	if ( ! isStop )	y -= speed;						// タイトルが止まっていなければ上がる
}


void Title::Draw()
{
	DrawRotaGraph(x, y, 2.0, 0.0, hg_title, true);
}