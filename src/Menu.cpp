#include "Menu.hpp"
#include "Main.hpp"
#include "Keyboard.hpp"
#include "DebugMode.hpp"

#include <iterator>
#include <algorithm>


Menu::Menu(ISceneChanger* changer)
	: BaseScene		(changer)
	, title			(new Title)
	, graphic		(new Graphic)
	, score			(new Score)
	, c_game		(new Counter(100))
	, X				(280)
	, SPACE			(16)
	, X_CUR			(262)
	, ALL_SELECT_NUM(4)
	, isGame(false)
{
	h_twi		= LoadGraph   ("GRAPH/MENU/myacount.png");
	h_cursor	= LoadGraph   ("GRAPH/MENU/select.png");
	hs_move		= LoadSoundMem("SOUND/SE/select01.wav");
	hs_choice	= LoadSoundMem("SOUND/SE/choice01.wav");
	selectNum	= 0;
	ano_tri		= 0;

	// メニューセレクトの並び座標
	for (int i = 0; i != y.size(); ++i)		y.at(i) = 270 + (i * 30);
}


Menu::~Menu()
{
	DeleteGraph   (h_twi);
	DeleteGraph   (h_cursor);
	DeleteSoundMem(hs_choice);
	DeleteSoundMem(hs_move);
}


void Menu::Update()
{
	title->Update();													// タイトルロゴ
	if (!title->isStop)	return;											// 止まっていなければここはスルー

	const bool INPUT_VERTICAL = ( Keyboard_Get(KEY_INPUT_DOWN) == 1 || Keyboard_Get(KEY_INPUT_UP)     == 1 );
	const bool INPUT_OK       = ( Keyboard_Get(KEY_INPUT_Z)    == 1 || Keyboard_Get(KEY_INPUT_RETURN) == 1 );
	const bool PUSH_KEY_UP    = ( Keyboard_Get(KEY_INPUT_UP)   == 1 );
	const bool PUSH_KEY_DOWN  = ( Keyboard_Get(KEY_INPUT_DOWN) == 1 );
	const bool STATE_START    = selectNum == 0;
	const bool STATE_QUIT     = selectNum == 3;

	y_cur = y[selectNum] - 2;											// カーソルの位置
	if (INPUT_VERTICAL)	PlaySoundMem(hs_move, DX_PLAYTYPE_BACK);		// ♪カーソル音
	if (PUSH_KEY_UP)	selectNum = (selectNum + 3) % ALL_SELECT_NUM;	// カーソルの移動と選択項目
	if (PUSH_KEY_DOWN)	selectNum = (selectNum + 1) % ALL_SELECT_NUM;

	if(INPUT_OK)														// 決定キーを押したなら
	{
		if (STATE_START)
		{
			PlaySoundMem(hs_choice, DX_PLAYTYPE_BACK);				// ♪決定音
			isGame = true;
		}
		if (STATE_QUIT)		QuitGame();									// ゲームをやめる
	}

	if(isGame)
	{
		c_game->Update();

		if (c_game->isLast())	mSceneChanger->ChangeScene(eScene_Game);
	}

	score->Update();													// スコアデータ更新

	AnotherUpdate();													// It's a secret.
}


void Menu::Draw()
{
	title->Draw();													// タイトル
	if ( !title->isStop )	return;									// タイトルが止まってないならスルー	

	graphic->DrawMyString2(X, y[0], "START",    SPACE, true,  2.0);
	graphic->DrawMyString2(X, y[1], "CONTINUE", SPACE, true,  2.0);
	graphic->DrawMyString2(X, y[2], "CONFIG",   SPACE, true,  2.0);
	graphic->DrawMyString2(X, y[3], "QUIT",     SPACE, false, 2.0);

	// その他の情報
	graphic->DrawMyString2(140, 440, "SINCE 2015   SHOPPE YAHU!", SPACE, true, 2.0);
	DrawRotaGraph(320, 466, 2.0, 0.0, h_twi, true);					// ツイッター
	DrawRotaGraph(X_CUR, y_cur, 2.0, 0.0, h_cursor, true);			// カーソル
	score->Draw();													// スコア表示

	if (isGame)
	{
		DrawBox(0, 0, 640, 480, GetColor(0, 0, 0), TRUE);
		graphic->DrawMyString2(60, 180, "STANDBY READY?", 40, false, 4.0);
		graphic->DrawMyString2(140, 260, "GOOD LUCK!", 40, false, 4.0);
	}

	// TEST
	if (!DebugMode::isTest)	return;

	DrawFormatString(0, 50, GetColor(0, 255, 0), "ano_tri = %d", ano_tri);
}


void Menu::AnotherUpdate() 
{
	const bool KEY_PUSH_RIGHT	= ( Keyboard_Get(KEY_INPUT_RIGHT) == 1 );
	const bool KEY_PUSH_UP		= ( Keyboard_Get(KEY_INPUT_UP)    == 1 );
	const bool KEY_PUSH_DOWN	= ( Keyboard_Get(KEY_INPUT_DOWN)  == 1 );
	const bool KEY_PUSH_LEFT	= ( Keyboard_Get(KEY_INPUT_LEFT)  == 1 );


	if (KEY_PUSH_RIGHT && ano_tri < 4)						ano_tri++;
	if (KEY_PUSH_UP    && ano_tri < 8  && ano_tri >= 4)		ano_tri++;
	if (KEY_PUSH_DOWN  && ano_tri < 12 && ano_tri >= 8)		ano_tri++;
	if (KEY_PUSH_LEFT  && ano_tri < 16 && ano_tri >= 12)	ano_tri++;

	if(ano_tri == 16){
		mSceneChanger->ChangeScene(eScene_Another);
	}
}