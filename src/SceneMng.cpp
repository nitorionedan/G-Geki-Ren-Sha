#include <DxLib.h>
#include <cassert>

/* scenes */
#include "Config.hpp"
#include "Game.hpp"
#include "GameOver.hpp"
#include "Menu.hpp"
#include "Ranking.hpp"
#include "GameClear.hpp"

#include "SceneMng.hpp"
#include "Another.hpp"
#include "Keyboard.hpp"
#include "DebugMode.hpp"
#include "Sound.hpp"


SceneMng::SceneMng() :	mNextScene(eScene::none){
	mScene = static_cast<BaseScene*> (new Menu(this));	// 最初のシーン
}


SceneMng::~SceneMng() {
	delete mScene;
}


//更新
void SceneMng::Update()
{			
	DebugMode::Update();

	if (mNextScene != eScene::none)								// 次のシーンがセットされていたら
	{    
		delete mScene;											// 現在のシーンの終了処理を実行
		switch (mNextScene)										// シーンによって処理を分岐
		{
		case eScene::menu:										// 次の画面がメニューなら
			mScene = static_cast<BaseScene*> (new Menu(this));	// メニュー画面のインスタンスを生成する
			break;												// 以下略
		case eScene::game:     mScene = static_cast<BaseScene*>(new Game(this)); break;
		case eScene::config:   mScene = static_cast<BaseScene*>(new Config(this)); break;
		case eScene::another:  mScene = static_cast<BaseScene*>(new Another(this)); break;
		case eScene::gameOver: mScene = static_cast<BaseScene*>(new GameOver(this)); break;
		case eScene::ranking:  mScene = static_cast<BaseScene*>(new Ranking(this)); break;
		case eScene::gameClear:  mScene = static_cast<BaseScene*>(new GameClear(this)); break;
		default:	assert(!"SceneMng::Update()");
		}
		mNextScene = eScene::none;								// 次のシーン情報をクリア
	}
	mScene->Update();											// シーンの更新

	// DEBUG ONLY
	if (Keyboard::Instance()->isPush(KEY_INPUT_F1))
		DebugMode::SwitchTest();
}


//描画
void SceneMng::Draw()
{
	DebugMode::Draw();
	mScene->Draw();
}


// 引数 nextScene にシーンを変更する
void SceneMng::ChangeScene(eScene NextScene)
{
	mNextScene = NextScene; // set next scene
	Sound::Stop();
}