#pragma once

#include "DxLib.h"


class DebugMode
{
public:
	static void SwitchTest();
	static void Update();
	static void Draw();

	static double enemySpeed;		// 敵速度の割合
	static double enemyShotSpeed;	// 敵弾速度の割合
	static double playerSpeed;		// 自機速度の割合
	static double playerShotSpeed;	// 自機弾速度の割合
	static bool isTest;				// デバッグモード中か？
	static bool isSuperEnemy;		// 無敵か？
	static bool isSuperPlayer;		// 無敵か？

private:
	DebugMode() {}
	~DebugMode() {}

	static bool isConfig;					// デバッグ用の設定モード中か？
};