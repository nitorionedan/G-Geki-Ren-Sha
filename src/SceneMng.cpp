#include <DxLib.h>

#include "Config.hpp"
#include "Game.hpp"
#include "GameOver.hpp"
#include "Menu.hpp"
#include "SceneMng.hpp"
#include "Another.hpp"
#include "Keyboard.hpp"
#include "DebugMode.hpp"


SceneMng::SceneMng() :	mNextScene(eScene_None){
	mScene = static_cast<BaseScene*> (new Game(this));	// 最初のシーン
}


SceneMng::~SceneMng()
{
	delete mScene;
}


//更新
void SceneMng::Update()
{			
	DebugMode::Update();

	if (mNextScene != eScene_None)								// 次のシーンがセットされていたら
	{    
		delete mScene;											// 現在のシーンの終了処理を実行
		switch (mNextScene)										// シーンによって処理を分岐
		{
		case eScene_Menu:										// 次の画面がメニューなら
			mScene = static_cast<BaseScene*> (new Menu(this));	// メニュー画面のインスタンスを生成する
			break;												// 以下略
		case eScene_Game:
			mScene = static_cast<BaseScene*> (new Game(this));
			break;
		case eScene_Config:
			mScene = static_cast<BaseScene*> (new Config(this));
			break;
		case eScene_Another:
			mScene = static_cast<BaseScene*> (new Another(this));
			break;
		case eScene_GameOver:
			mScene = static_cast<BaseScene*> (new GameOver(this));
			break;
		}
		mNextScene = eScene_None;								// 次のシーン情報をクリア
	}
	mScene->Update();											// シーンの更新

	// DEBUG ONLY
	if (Keyboard::Instance()->isPush(KEY_INPUT_F1))	DebugMode::SwitchTest();
}


//描画
void SceneMng::Draw()
{
	DebugMode::Draw();
	mScene->Draw();
}


// 引数 nextScene にシーンを変更する
void SceneMng::ChangeScene(eScene NextScene){	mNextScene = NextScene;	}	// 次のシーンをセット