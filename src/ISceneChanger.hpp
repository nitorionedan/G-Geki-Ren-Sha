#pragma once


enum class eScene
{
	menu,	  // メニュー画面
	game,	  // ゲーム画面
	gameOver, // ガメオベラ
	config,	  // 設定画面
	another,  // 秘密
	ranking,
	gameClear,

	none,	  // 無し
};


//シーンを変更するためのインターフェイスクラス
class ISceneChanger
{
public:
	virtual ~ISceneChanger() = 0;
	virtual void ChangeScene(eScene NextScene) = 0; //指定シーンに変更する
};