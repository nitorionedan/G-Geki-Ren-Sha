/*
	激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激
	激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激連激激激激激激激激激激激激激激激激激激激激激激激激激激激激激
	激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激
	激激激激激激激激激激激激激激激死ぬがよい。激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激
	激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激

	　　　　　　激激　　激激激激　激　　激　激激激　激激激　　激激激激　激　　　激　　激激激　激　　　激　　激激
	　　　　　激　　　　激　　　　激　激　　　激　　激　　激　激　　　　激激　　激　激　　　　激　　　激　激　　激
	　　　　　激　激激　激激激　　激激　　　　激　　激激激　　激激激　　激　激　激　　激激　　激激激激激　激激激激
	　　　　　激　　激　激　　　　激　激　　　激　　激　　激　激　　　　激　　激激　　　　激　激　　　激　激　　激
	　　　　　　激激　　激激激激　激　　激　激激激　激　　激　激激激激　激　　　激　激激激　　激　　　激　激　　激 68k

　　　　　　　　　　　　　　　　　　　　　　　～　大好きな超連射68kのために　～												  
　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　
	 激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激
	 激激激激激激射激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激
	 激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激激超

     // どこかに「超」「連」「射」が隠れているよ！探してみよう！
*/
#include "DxLib.h"

#include "Keyboard.hpp"
#include "SceneMng.hpp"
#include "DebugMode.hpp"

#include <memory>
#include <time.h>


static int  FrameStartTime;			// 60fps固定専用
static int  FPS;
static bool ScSizeFrag = false;		// 画面モード変更用
static bool quit       = false;		// 強制終了フラグ

void QuitGame();					// ゲーム終了伝達関数

void message_box()
{
	int flag;

	flag = MessageBox(
		NULL,
		TEXT("フルスクリーンモードで起動しますか？\n（F5キーでも変更できます。）"),
		TEXT("スクリーン設定"),
		MB_YESNOCANCEL | MB_ICONQUESTION);
	
	if (flag == IDNO)
	{
		ScSizeFrag = true;
//		SetUseBackBufferTransColorFlag(true); // 全部透明化フラグ
	}
	if (flag == IDCANCEL)	QuitGame();
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	message_box();																				// ウィンドウスタイル質問
	SetWindowIconID(444);
	SetGraphMode(640, 480, 32), ChangeWindowMode(ScSizeFrag), DxLib_Init();						// ウィンドウ初期設定(VGA),DxLib起動
	SetDrawScreen(DX_SCREEN_BACK);																// 裏画面処理
	SetMainWindowText("激連射");																// タイトルを設定
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);											// 画面モード変更後、素材メモリをリセットしない
	SetEmulation320x240(TRUE);																	// 320x240の解像度にする
	SetWaitVSyncFlag(FALSE);																	// 垂直同期信号を待たない
	FrameStartTime = GetNowCount();																// 開始時間を設定
	FPS = 60;
	std::unique_ptr<SceneMng> sceneMng(new SceneMng);											// シーン管理
	SRand((unsigned)time(NULL));																// 乱数シード
	LoadPauseGraph("GRAPH/Cover.png");															// 非アクティブ状態時の画像

	// ゲーム--------------------------------------------------------------------------------------
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && !quit)
	{
		while (GetNowCount() - FrameStartTime < 1000 / FPS) {}									// 1/60 秒まで待つ
		FrameStartTime = GetNowCount();															// 現在のカウントを保存

		sceneMng->Update();																		// 現在のシーンデータを更新
		Keyboard_Update();																		// キー入力データ更新

		if (Keyboard_Get(KEY_INPUT_ESCAPE) == 1)	break;										// [Esc]で疑似強制終了
		if (Keyboard_Get(KEY_INPUT_F5) == 1)		ChangeWindowMode(ScSizeFrag = !ScSizeFrag);	// 画面モード変更
		if (Keyboard_Get(KEY_INPUT_F) == 1)	FPS = 30;											// ゆっくり
		if (Keyboard_Get(KEY_INPUT_G) == 1)	FPS = 60;											// 普通

		sceneMng->Draw();																		// 現在のシーンを描画
		DebugMode::Draw();
	}
	LoadPauseGraph(NULL);
	DxLib_End();																				// ＤＸライブラリ使用の終了処理
	return 0;																					// ソフトの終了 
}


void QuitGame(){
	quit = true;
}