#pragma once

#include "DxLib.h"

#include "BaseScene.hpp"
#include "Graphic.hpp"
#include "Keyboard.hpp"
#include "Score.hpp"
#include "Title.hpp"
#include "Counter.hpp"

#include <memory>
#include <array>


// メニューのカーソルクラス
/*
class Title_Select
{
public:
	int hg;
	int hs_select;	// select SE
	int hs_choice;
	int y;
	int selectNum;

	const int x;

public:
	Title_Select()
		: x(262)
	{
	}
	~Title_Select()
	{
		DeleteGraph(hg);
		DeleteSoundMem(hs_select);
		DeleteSoundMem(hs_choice);
	}
};
*/


//メニュー画面クラス
class Menu : public BaseScene
{
public:
	Menu(ISceneChanger* changer);
	~Menu();

private:
	void Update() override;
	void Draw() override;
	void AnotherUpdate();		// 秘密の扉

	std::unique_ptr<Title>   title;
	std::unique_ptr<Graphic> graphic;
	std::unique_ptr<Score>   score;
	std::unique_ptr<Counter> c_game;

	const int X;				// 項目のｘ座標
	const int SPACE;			// 文字と文字の間隔
	const int X_CUR;			// カーソルのｘ座標
	const int ALL_SELECT_NUM;	// メニューの項目数

	std::array<int, 4> y;		// 項目のｙ座標
	int y_cur;					// カーソルの座標
	int selectNum;				// 現在の選択番号
	int h_twi, h_cursor;		// 画像用ハンドル
	int hs_choice, hs_move;		// サウンド用ハンドル
	int ano_tri;				// Another Trigger
	bool isGame;
};