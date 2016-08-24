//------------------------------------------------------------------------------
/// @file
/// @brief    Keyboard.hppの実装
/// @author   PG班 矢風！
///
/// @copyright  Copyright 2016 Shohei.Y
/// @attention  このファイルの利用は、同梱のREADMEにある
///             利用条件に従ってください

//------------------------------------------------------------------------------

#include "DxLib.h"

#include "Keyboard.hpp"


static int m_Key[256];			  // キーの入力状態格納用変数
static int n_Key[256];


// キーの入力状態更新
void Keyboard_Update()
{
	char tmpKey[256];             // 現在のキーの入力状態を格納する
	GetHitKeyStateAll(tmpKey);    // 全てのキーの入力状態を得る

	for (int i = 0; i < 256; i++)
	{
		if (tmpKey[i] != 0)     // i番のキーコードに対応するキーが押されていたら
			m_Key[i]++;         // 加算
		else                    // 押されていなければ
			m_Key[i] = 0;       // 0にする
	}
}


// KeyCodeのキーの入力状態を取得する
int Keyboard_Get(int KeyCode){
	return m_Key[KeyCode];		  // KeyCodeの入力状態を返す
}