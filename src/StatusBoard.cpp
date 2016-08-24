#include "DxLib.h"

#include "StatusBoard.hpp"
#include "Player.hpp"
#include "Keyboard.hpp"
#include "DebugMode.hpp"


StatusBoard::StatusBoard()
	: POS_BOARD_X(320)
	, POS_BOARD_Y(458)
	, POS_BOMB_X(40)
	, POS_BOMB_Y(466)
	, POS_KANJI_X(510)
	, POS_KANJI_Y(458)
	, POS_LIFE_X(40)
	, POS_LIFE_Y(446)
	, POS_RENSHA_X(249)
	, POS_RENSHA_Y(465)
{
	gh_board = LoadGraph("GRAPH/GAME/board.png");
	gh_bomb = LoadGraph("GRAPH/GAME/ibomb.png");
	gh_life = LoadGraph("GRAPH/GAME/ilife.png");
	gh_rensha = LoadGraph("GRAPH/GAME/rensha1.png");
	LoadDivGraph("GRAPH/GAME/ikanji21x19.png", 5, 5, 1, 21, 19, gh_kanji);
}


StatusBoard::~StatusBoard()
{
	DeleteGraph(gh_board);
	DeleteGraph(gh_bomb);
	DeleteGraph(gh_life);
	DeleteGraph(gh_rensha);
	for(int& gh : gh_kanji)	DeleteGraph(gh);
}


void StatusBoard::Update()
{
	// TEST------------------------------------------
	if (!DebugMode::isTest)	return;

	if(Keyboard_Get(KEY_INPUT_P) == 1)
		Player::Shift(true);
	if (Keyboard_Get(KEY_INPUT_O) == 1)
		Player::Shift(false);
}


void StatusBoard::Draw()
{
	DrawRotaGraph(POS_BOARD_X, POS_BOARD_Y, 2.0, 0.0, gh_board, true);
	DrawRotaGraph(POS_KANJI_X, POS_KANJI_Y, 2.0, 0.0, gh_kanji[Player::GetShiftLevel()], true);

	// -----------------------------------------------------------------------------------------
	switch (Player::GetShiftLevel())
	{
	case 0:	break;
	case 1: break;
	case 2:
		for (int i = 0; i < 40; i++)
			DrawColorGraph(POS_RENSHA_X + (i * 4), POS_RENSHA_Y, 2.0, gh_rensha, 255, 255, 255);
		break;
	case 3:
		for (int i = 0; i < 40; i++)
			DrawColorGraph(POS_RENSHA_X + (i * 4), POS_RENSHA_Y, 2.0, gh_rensha, 0, 150, 255);
		break;
	case 4:
		for (int i = 0; i < 40; i++)
			DrawColorGraph(POS_RENSHA_X + (i * 4), POS_RENSHA_Y, 2.0, gh_rensha, 0, 255, 0);
		break;
	}

	switch (Player::GetShiftLevel())
	{
	case 0: break;
	case 1:
		for (int i = 0; i < Player::GetRensha(); i++)				// ˜AŽËƒQ[ƒW
			DrawRotaGraph(POS_RENSHA_X + (i * 4), POS_RENSHA_Y, 2.0, 0.0, gh_rensha, true);
		break;
	case 2:
		for (int i = 0; i < Player::GetRensha(); i++)
			DrawColorGraph(POS_RENSHA_X + (i * 4), POS_RENSHA_Y, 2.0, gh_rensha, 0, 150, 255);
		break;
	case 3:
		for (int i = 0; i < Player::GetRensha(); i++)
			DrawColorGraph(POS_RENSHA_X + (i * 4), POS_RENSHA_Y, 2.0, gh_rensha, 0, 255, 0);
		break;
	case 4:
		for (int i = 0; i < Player::GetRensha(); i++)
			DrawColorGraph(POS_RENSHA_X + (i * 4), POS_RENSHA_Y, 2.0, gh_rensha, 255, 0, 0);
		break;
	}

	// -----------------------------------------------------------------------------------------
	for (int i = 0; i < Player::GetLife() - 1; i++)
		DrawRotaGraph(POS_LIFE_X + (i * 18), POS_LIFE_Y, 2.0, 0.0, gh_life, true);
	for (int i = 0; i < Player::GetBombNum(); i++)
		DrawRotaGraph(POS_BOMB_X + (i * 18), POS_BOMB_Y, 2.0, 0.0, gh_bomb, true);
}
