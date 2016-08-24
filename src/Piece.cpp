//------------------------------------------------------------------------------
/// @file
/// @brief    Piece.hppの実装
/// @author   PG班 矢風！
///
/// @copyright  Copyright 2016 Shohei.Y
/// @attention  このファイルの利用は、同梱のREADMEにある
///             利用条件に従ってください

//------------------------------------------------------------------------------

#include "DxLib.h"

#include "Piece.hpp"
#include "Bezier.hpp"

#include <cmath>


Piece::Piece(ePiece_t type_)
	: type(type_)
{
	c_play = new Counter*[PIECE_ALL_NUM];
	for (int i = 0; i < PIECE_ALL_NUM; i++)	c_play[i] = new Counter(131);

	switch (type)
	{
	case ePiece_SmallGray: LoadDivGraph("GRAPH/GAME/EFFECT/small1.png", 8, 4, 2, 4, 4, gh_small1);		break;
	case ePiece_SmallCoin: LoadDivGraph("GRAPH/GAME/EFFECT/small6.png", 5, 5, 1, 4, 4, gh_small6);		break;
	case ePiece_SmallSquare: LoadDivGraph("GRAPH/GAME/EFFECT/small5.png", 4, 4, 1, 4, 4, gh_small5);	break;
	case ePiece_Smalls: LoadDivGraph("GRAPH/GAME/EFFECT/smalls.png", 8, 4, 2, 4, 4, gh_smalls);			break;
	case ePiece_BigNut: LoadDivGraph("GRAPH/GAME/EFFECT/big1.png", 8, 4, 2, 8, 8, gh_big1);				break;
	case ePiece_BigPlate: LoadDivGraph("GRAPH/GAME/EFFECT/big2.png", 14, 7, 2, 8, 8, gh_big2);			break;
	case ePiece_BigSquare: LoadDivGraph("GRAPH/GAME/EFFECT/big3.png", 6, 3, 2, 8, 8, gh_big3);			break;
	case ePiece_BigPlate3: LoadDivGraph("GRAPH/GAME/EFFECT/big4.png", 14, 7, 2, 8, 8, gh_big5);			break;
	case ePiece_Bigs: LoadDivGraph("GRAPH/GAME/EFFECT/bigs.png", 8, 4, 2, 8, 8, gh_bigs);				break;
	case ePiece_Special:
		gh_burger	= LoadGraph("GRAPH/GAME/EFFECT/burger.png");
		gh_kit		= LoadGraph("GRAPH/GAME/EFFECT/kitic.png");	break;
	}
	angle.fill(0.0);
	isSlow.fill(false);
}


Piece::~Piece()
{
	for (int i = 0; i < PIECE_ALL_NUM; i++)	delete c_play[i];
	delete[] c_play;

	switch (type)
	{
	case ePiece_SmallGray:
		for (int i = 0; i < sizeof(gh_small1) / sizeof(gh_small1[0]); i++)	DeleteGraph(gh_small1[i]);
		break;
	case ePiece_SmallCoin: 
		for (int i = 0; i < sizeof(gh_small6) / sizeof(gh_small6[0]); i++)	DeleteGraph(gh_small6[i]);
		break;
	case ePiece_SmallSquare:
		for (int i = 0; i < sizeof(gh_small5) / sizeof(gh_small5[0]); i++)	DeleteGraph(gh_small5[i]);
		break;
	case ePiece_Smalls:
		for (int i = 0; i < sizeof(gh_smalls) / sizeof(gh_smalls[0]); i++)	DeleteGraph(gh_smalls[i]);
		break;
	case ePiece_BigNut:
		for (int i = 0; i < sizeof(gh_big1) / sizeof(gh_big1[0]); i++)	DeleteGraph(gh_big1[i]);
		break;
	case ePiece_BigPlate:
		for (int i = 0; i < sizeof(gh_big2) / sizeof(gh_big2[0]); i++)	DeleteGraph(gh_big2[i]);
		break;
	case ePiece_BigSquare: 
		for (int i = 0; i < sizeof(gh_big3) / sizeof(gh_big3[0]); i++)	DeleteGraph(gh_big3[i]);
		break;
	case ePiece_BigPlate3: 
		for (int i = 0; i < sizeof(gh_big5) / sizeof(gh_big5[0]); i++)	DeleteGraph(gh_big5[i]);
		break;
	case ePiece_Bigs:
		for (int i = 0; i < sizeof(gh_bigs) / sizeof(gh_bigs[0]); i++)	DeleteGraph(gh_bigs[i]);
		break;
	case ePiece_Special:
		DeleteGraph(gh_burger);
		DeleteGraph(gh_kit);
		break;
	}
}


void Piece::Update()
{
	for (int i = 0; i < PIECE_ALL_NUM; i++)
	{
		if (isPlay[i] == false)	continue;
		c_play[i]->Update();

		Move(i);

		// リセット
		if(c_play[i]->isLast())
		{
			c_play[i]->Reset();
			isPlay[i] = false;
			isSlow[i] = false;
			angle[i] = 0.0;
			red[i] = 0;
			green[i] = 0;
			blue[i] = 0;
		}
	}
}


void Piece::Draw()
{
	for (int i = 0; i < PIECE_ALL_NUM; i++)
	{
		if (isPlay[i] == false) continue;

		Draw_Piece(i);
	}
}


void Piece::Play(const double& PosX, const double& PosY, const double& V_ANGLE, const double& VEC, const int& FSPEED, const double& SPIN, const double& BRAKE, int R, int G, int B)
{
	for (int i = 0; i < PIECE_ALL_NUM; i++)
	{
		if (isPlay[i] == true) continue;
		isPlay[i]	= true;
		pos[i].SetVecor2D(PosX, PosY);
		vangle[i]	= V_ANGLE;
		vec[i]		= VEC;
		fspeed[i]	= FSPEED;
		spin[i]		= SPIN;
		brake[i]	= BRAKE;
		rand[i] = GetRand(7);

		// 色
		red[i]		= R;
		green[i]	= G;
		blue[i]		= B;
		break;
	}
}


void Piece::PlayAnime(const double & PosX, const double & PosY)
{
}


void Piece::Draw_Piece(const int & i)
{
	SetDrawBright(red[i], green[i], blue[i]);

	if(isSlow[i] == true && c_play[i]->Remainder(4) >= 2)
	{
		SetDrawBright(255, 255, 255);
		return;
	}

	switch (type)
	{
	case ePiece_SmallGray:
		
		for (int j = 0; j < sizeof(gh_small1) / sizeof(gh_small1[0]); j++)
		{
			if (c_play[i]->Remainder(16) == (j * 2) || c_play[i]->Remainder(16) == (j * 2) + 1)
			{
				DrawRotaGraph(pos[i].x, pos[i].y, 1.0, angle[i], gh_small1[j], true);
				break;
			}
		}break;
	case ePiece_SmallCoin:
		for (int j = 0; j < sizeof(gh_small6) / sizeof(gh_small6[0]); j++)
		{
			if (c_play[i]->Remainder(10) == (j * 2) || c_play[i]->Remainder(10) == (j * 2) + 1)
			{
				DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_small6[j], true);
				break;
			}
		}break;
	case ePiece_SmallSquare:
		for (int j = 0; j < sizeof(gh_small5) / sizeof(gh_small5[0]); j++)
		{
			if (c_play[i]->Remainder(8) == (j * 2) || c_play[i]->Remainder(8) == (j * 2) + 1)
			{
				DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_small5[j], true);
				break;
			}
		}break;
	case ePiece_Smalls:
		for (int j = 0; j < sizeof(gh_smalls) / sizeof(gh_smalls[0]); j++)
		{
			if (c_play[i]->Remainder(16) == (j * 2) || c_play[i]->Remainder(16) == (j * 2) + 1)
			{
				DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_smalls[ rand[i] ], true);
				break;
			}
		}break;
	case ePiece_BigNut:
		for (int j = 0; j < sizeof(gh_big1) / sizeof(gh_big1[0]); j++)
		{
			if (c_play[i]->Remainder(16) == (j * 2) || c_play[i]->Remainder(16) == (j * 2) + 1)
			{
				DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_big1[j], true);
				break;
			}
		}break;
	case ePiece_BigPlate:
		for (int j = 0; j < sizeof(gh_big2) / sizeof(gh_big2[0]); j++)
		{
			if (c_play[i]->Remainder(28) == (j * 2) || c_play[i]->Remainder(28) == (j * 2) + 1)
			{
				DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_big2[j], true);
				break;
			}
		}break;
	case ePiece_BigSquare:
		for (int j = 0; j < sizeof(gh_big3) / sizeof(gh_big3[0]); j++)
		{
			if (c_play[i]->Remainder(12) == (j * 2) || c_play[i]->Remainder(12) == (j * 2) + 1)
			{
				DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_big3[j], true);
				break;
			}
		}break;
	case ePiece_BigPlate3:
		for (int j = 0; j < sizeof(gh_big5) / sizeof(gh_big5[0]); j++)
		{
			if (c_play[i]->Remainder(28) == (j * 2) || c_play[i]->Remainder(28) == (j * 2) + 1)
			{
				DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_big5[j], true);
				break;
			}
		}break;
	case ePiece_Bigs:
		for (int j = 0; j < sizeof(gh_bigs) / sizeof(gh_bigs[0]); j++)
		{
			if (c_play[i]->Remainder(16) == (j * 2) || c_play[i]->Remainder(16) == (j * 2) + 1)
			{
				DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_bigs[ rand[i] ], true);
				break;
			}
		}break;
	case ePiece_Special:
		if(	rand[i] >= 4)	DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_kit, true);
		if (rand[i] <= 3)	DrawRotaGraph(pos[i].x, pos[i].y, 2.0, angle[i], gh_burger, true);
		break;
	}
	SetDrawBright(255, 255, 255);
}


void Piece::Move(const int & INDEX)
{
	if (c_play[INDEX]->GetNowcount() == 80)	isSlow[INDEX] = true;

	pos[INDEX].x += std::cos(vangle[INDEX]) * vec[INDEX];	// 移動
	pos[INDEX].y += std::sin(vangle[INDEX]) * vec[INDEX];

	angle[INDEX] += spin[INDEX];							// 回す
	if (vec[INDEX] > 0.5)	vec[INDEX] -= brake[INDEX];
}
