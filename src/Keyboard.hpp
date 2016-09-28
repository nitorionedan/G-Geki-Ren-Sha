#pragma once
#include "Singleton.hpp"
#include <DxLib.h>


struct KeyCode_t
{
	const int Right  = KEY_INPUT_RIGHT;
	const int Left   = KEY_INPUT_LEFT;
	const int Up     = KEY_INPUT_UP;
	const int Down   = KEY_INPUT_DOWN;
	const int Return = KEY_INPUT_RETURN;
	const int C      = KEY_INPUT_C;
	const int O      = KEY_INPUT_O;
	const int P      = KEY_INPUT_P;
	const int Q      = KEY_INPUT_Q;
	const int X      = KEY_INPUT_X;
	const int Z      = KEY_INPUT_Z;
};


class Input
{
public:
	static const KeyCode_t KeyCode;
};


class Keyboard : public Singleton<Keyboard>
{
	Keyboard();
	friend Singleton<Keyboard>;

public:
	bool Update();
	int GetDown(int keyCode);
	int GetUp(int keyCode);
	bool isDown(int keyCode);
	bool isPush(int keyCode);

private:
	// keyCodeは有効なキーコードか？
	bool IsAvailableCode(int keyCode);
	static const int KEY_NUM = 256;
	int mKeyDownCount[KEY_NUM];
	int mKeyUpCount[KEY_NUM];
};