#include "GameClear.hpp"
#include "FileDef.h"
#include "Keyboard.hpp"
#include <DxLib.h>

namespace
{
	constexpr int FEED_TIME = 120;
	constexpr int FEED_STAYTIME = 120;
}


GameClear::GameClear(ISceneChanger * changer)
	: BaseScene(changer)
	, time(0)
	, time_in(0)
{
	gh = LoadGraph(MyFile::Gr::GAME_CLEAR);
}

GameClear::~GameClear()
{
	DeleteGraph(gh);
}

void GameClear::Update()
{
	time++;

	if (time < 121)
		time_in += 255 / 120;

	if (time < 121 + FEED_STAYTIME)	return;

	time_in -= 255 / 120;

	if (time_in == 0)
		mSceneChanger->ChangeScene(eScene::menu);

	/* skip cmd */
	if (Keyboard::Instance()->isPush(KEY_INPUT_Z))
		mSceneChanger->ChangeScene(eScene::menu);
}

void GameClear::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, time_in);
	DrawRotaGraph(320, 240, 2.0, 0.0, gh, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
