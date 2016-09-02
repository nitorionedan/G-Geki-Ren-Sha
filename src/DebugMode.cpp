#include "DxLib.h"

#include "DebugMode.hpp"
#include "Keyboard.hpp"
#include "Game.hpp"


double	DebugMode::enemySpeed		= 1.;
double	DebugMode::enemyShotSpeed	= 1.;
double	DebugMode::playerSpeed		= 1.;
double	DebugMode::playerShotSpeed	= 1.;
bool	DebugMode::isTest			= false;
bool	DebugMode::isSuperEnemy		= false;
bool	DebugMode::isSuperPlayer	= false;
bool	DebugMode::isConfig			= false;


void DebugMode::SwitchTest()
{
	isTest = !isTest;
}


void DebugMode::Update()
{
	if (!isTest)	return;

	if (Keyboard::Instance()->isPush(KEY_INPUT_F2))
	{
		isConfig = !isConfig;
	}

	if(isConfig){}
}


void DebugMode::Draw()
{
	DrawFormatString(0, 20, GetColor(255, 0, 0), "DEBUG_MODE_NOW : OPTION[F2]");
}
