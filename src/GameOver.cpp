#include "DxLib.h"

#include "GameOver.hpp"
#include "Keyboard.hpp"


static const int FEED_TIME = 120;
static const int FEED_STAYTIME = 120;


GameOver::GameOver(ISceneChanger * changer)
	: BaseScene(changer)
	, gh(LoadGraph("GRAPH/GAME/GameOver.png"))
	, time(0)
	, time_in(0)
{
}


GameOver::~GameOver()
{
	DeleteGraph(gh);
}


void GameOver::Update()
{
	time++;

	if (time < 121)
		time_in += 255 / 120;

	if (time < 121 + FEED_STAYTIME)	return;

	time_in -= 255 / 120;
	
	if (time_in == 0)	mSceneChanger->ChangeScene(eScene_Menu);
	
	if (Keyboard_Get(KEY_INPUT_Z) == 1)	mSceneChanger->ChangeScene(eScene_Menu);
}


void GameOver::Draw()
{
	SetDrawBright(time_in, time_in, time_in);
	DrawRotaGraph(320, 240, 2.0, 0.0, gh, true);
	SetDrawBright(255, 255, 255);
}
