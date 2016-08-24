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

#include "PieceEffect.hpp"


PieceEffect::PieceEffect()
	: SPEED1(3.)
	, SPEED2(4.)
	, SPEED3(5.8)
	, SPEED4(7.)
	, SPEED5(5.)
	, SPEED6(3.8) // ----
	, SPEED7(3.)
	, SPEED8(4.)
	, SPEED9(5.8)
	, SPEED10(7.)
	, SPEED11(5.)
	, SPEED12(3.8)
	
	, ANGLE1(DX_PI * 1.4)
	, ANGLE2(DX_PI * 1.05) // 1.3
	, ANGLE3(DX_PI * 1.05)
	, ANGLE4(DX_PI)
	, ANGLE5(DX_PI * 0.9)
	, ANGLE6(DX_PI * 0.7) // ---
	, ANGLE7(DX_PI * 0.4)
	, ANGLE8(DX_PI * 0.3)
	, ANGLE9(DX_PI * 0.05)
	, ANGLE10(0.0)
	, ANGLE11(DX_PI * 1.9)
	, ANGLE12(DX_PI * 1.9)

	, BRAKE1(0.2)
	, BRAKE2(0.1)
	, BRAKE3(0.1)
	, BRAKE4(0.1)
	, BRAKE5(0.1)
	, BRAKE6(0.2) // ---Inasd
	, BRAKE7(0.2)
	, BRAKE8(0.1)
	, BRAKE9(0.1)
	, BRAKE10(0.1)
	, BRAKE11(0.1)
	, BRAKE12(0.1)
{
	piece = new Piece*[10];
	piece[0] = new Piece(ePiece_SmallGray);
	piece[1] = new Piece(ePiece_SmallCoin);
	piece[2] = new Piece(ePiece_SmallSquare);
	piece[3] = new Piece(ePiece_Smalls);
	piece[4] = new Piece(ePiece_BigNut);
	piece[5] = new Piece(ePiece_BigPlate);
	piece[6] = new Piece(ePiece_BigSquare);
	piece[7] = new Piece(ePiece_BigPlate3);
	piece[8] = new Piece(ePiece_Bigs);
	piece[9] = new Piece(ePiece_Special);
}


PieceEffect::~PieceEffect()

{
	for (int i = 0; i < 10; i++)	delete piece[i];
	delete[] piece;
}


void PieceEffect::Update()
{
	for (int i = 0; i < 10; i++)	piece[i]->Update();
}


void PieceEffect::Draw()
{
	for (int i = 0; i < 10; i++)	piece[i]->Draw();
}


void PieceEffect::PlayAnime(const double & PosX, const double & PosY, const double & ANGLE, eSpread_t kind_)
{
	double rand_spin;
	int rand_type;

	switch (kind_)
	{
	case eSpread_SmallGrey:
		rand_spin = ((double)GetRand(20) - 10.) / 100.;	// 回転値をランダムに取得 (-0.10 ~ 0.1)
		piece[0]->Play(PosX, PosY, ANGLE + ANGLE1, SPEED1, 2., rand_spin, BRAKE1, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[0]->Play(PosX, PosY, ANGLE + ANGLE2, SPEED2, 2., rand_spin, BRAKE2, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[0]->Play(PosX, PosY, ANGLE + ANGLE3, SPEED3, 2., rand_spin, BRAKE3, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[0]->Play(PosX, PosY, ANGLE + ANGLE4, SPEED4, 2., rand_spin, BRAKE4, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[0]->Play(PosX, PosY, ANGLE + ANGLE5, SPEED5, 2., rand_spin, BRAKE5, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[0]->Play(PosX, PosY, ANGLE + ANGLE6, SPEED6, 2., rand_spin, BRAKE6, 255, 255, 255);

		rand_spin = ((double)GetRand(20) - 10.) / 100.;	// 回転値をランダムに取得 (-0.10 ~ 0.1)
		piece[0]->Play(PosX, PosY, ANGLE + ANGLE7, SPEED7, 2., rand_spin, BRAKE7, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[0]->Play(PosX, PosY, ANGLE + ANGLE8, SPEED8, 2., rand_spin, BRAKE8, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[0]->Play(PosX, PosY, ANGLE + ANGLE9, SPEED9, 2., rand_spin, BRAKE9, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[0]->Play(PosX, PosY, ANGLE + ANGLE10, SPEED10, 2., rand_spin, BRAKE10, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[0]->Play(PosX, PosY, ANGLE + ANGLE11, SPEED11, 2., rand_spin, BRAKE11, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[0]->Play(PosX, PosY, ANGLE + ANGLE12, SPEED12, 2., rand_spin, BRAKE12, 255, 255, 255);
		break;
	case eSpread_SmallBlue:
		rand_spin = ((double)GetRand(20) - 10.) / 100.;	// 回転値をランダムに取得 (-0.10 ~ 0.1)
		piece[0]->Play(PosX, PosY, ANGLE + ANGLE1, SPEED1, 2., rand_spin, BRAKE1, 0, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[0]->Play(PosX, PosY, ANGLE + ANGLE2, SPEED2, 2., rand_spin, BRAKE2, 0, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[0]->Play(PosX, PosY, ANGLE + ANGLE3, SPEED3, 2., rand_spin, BRAKE3, 0, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[0]->Play(PosX, PosY, ANGLE + ANGLE4, SPEED4, 2., rand_spin, BRAKE4, 0, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[0]->Play(PosX, PosY, ANGLE + ANGLE5, SPEED5, 2., rand_spin, BRAKE5, 0, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[0]->Play(PosX, PosY, ANGLE + ANGLE6, SPEED6, 2., rand_spin, BRAKE6, 0, 255, 255);

		rand_spin = ((double)GetRand(20) - 10.) / 100.;	// 回転値をランダムに取得 (-0.10 ~ 0.1)
		piece[0]->Play(PosX, PosY, ANGLE + ANGLE7, SPEED7, 2., rand_spin, BRAKE7, 0, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[0]->Play(PosX, PosY, ANGLE + ANGLE8, SPEED8, 2., rand_spin, BRAKE8, 0, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[0]->Play(PosX, PosY, ANGLE + ANGLE9, SPEED9, 2., rand_spin, BRAKE9, 0, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[0]->Play(PosX, PosY, ANGLE + ANGLE10, SPEED10, 2., rand_spin, BRAKE10, 0, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[0]->Play(PosX, PosY, ANGLE + ANGLE11, SPEED11, 2., rand_spin, BRAKE11, 0, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[0]->Play(PosX, PosY, ANGLE + ANGLE12, SPEED12, 2., rand_spin, BRAKE12, 0, 255, 255);
		break;
	case eSpread_SmallOrange:
		break;
	case eSpread_SmallRed:
		break;
	case eSpread_SmallAll:
		rand_spin = ((double)GetRand(20) - 10.) / 100.;	// 回転値をランダムに取得 (-0.10 ~ 0.1)
		rand_type = GetRand(3);
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE1, SPEED1, 2., rand_spin, BRAKE1, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(3);
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE2, SPEED2, 2., rand_spin, BRAKE2, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(3);
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE3, SPEED3, 2., rand_spin, BRAKE3, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(3);
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE4, SPEED4, 2., rand_spin, BRAKE4, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(3);
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE5, SPEED5, 2., rand_spin, BRAKE5, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(3);
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE6, SPEED6, 2., rand_spin, BRAKE6, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;	// 回転値をランダムに取得 (-0.10 ~ 0.1)
		rand_type = GetRand(3);
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE7, SPEED7, 2., rand_spin, BRAKE7, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(3);
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE8, SPEED8, 2., rand_spin, BRAKE8, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(3);
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE9, SPEED9, 2., rand_spin, BRAKE9, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(3);
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE10, SPEED10, 2., rand_spin, BRAKE10, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(3);
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE11, SPEED11, 2., rand_spin, BRAKE11, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(3);
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE12, SPEED12, 2., rand_spin, BRAKE12, 255, 255, 255);
		break;
	case eSpread_Smalls:
		rand_spin = ((double)GetRand(20) - 10.) / 100.;	// 回転値をランダムに取得 (-0.10 ~ 0.1)
		piece[3]->Play(PosX, PosY, ANGLE + ANGLE1, SPEED1, 2., rand_spin, BRAKE1, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(9);
		piece[3]->Play(PosX, PosY, ANGLE + ANGLE2, SPEED2, 2., rand_spin, BRAKE2, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(9);
		piece[3]->Play(PosX, PosY, ANGLE + ANGLE3, SPEED3, 2., rand_spin, BRAKE3, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(9);
		piece[3]->Play(PosX, PosY, ANGLE + ANGLE4, SPEED4, 2., rand_spin, BRAKE4, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(9);
		piece[3]->Play(PosX, PosY, ANGLE + ANGLE5, SPEED5, 2., rand_spin, BRAKE5, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(9);
		piece[3]->Play(PosX, PosY, ANGLE + ANGLE6, SPEED6, 2., rand_spin, BRAKE6, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;	// 回転値をランダムに取得 (-0.10 ~ 0.1)
		rand_type = GetRand(9);
		piece[3]->Play(PosX, PosY, ANGLE + ANGLE7, SPEED7, 2., rand_spin, BRAKE7, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(9);
		piece[3]->Play(PosX, PosY, ANGLE + ANGLE8, SPEED8, 2., rand_spin, BRAKE8, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(9);
		piece[3]->Play(PosX, PosY, ANGLE + ANGLE9, SPEED9, 2., rand_spin, BRAKE9, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(9);
		piece[3]->Play(PosX, PosY, ANGLE + ANGLE10, SPEED10, 2., rand_spin, BRAKE10, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(9);
		piece[3]->Play(PosX, PosY, ANGLE + ANGLE11, SPEED11, 2., rand_spin, BRAKE11, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(9);
		piece[3]->Play(PosX, PosY, ANGLE + ANGLE12, SPEED12, 2., rand_spin, BRAKE12, 255, 255, 255);
		break;
	case eSpread_BigAll:
		rand_spin = ((double)GetRand(20) - 10.) / 100.;	// 回転値をランダムに取得 (-0.10 ~ 0.1)
		rand_type = GetRand(4) + 5;
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE1, SPEED1, 2., rand_spin, BRAKE1, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(4) + 5;
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE2, SPEED2, 2., rand_spin, BRAKE2, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(4) + 5;
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE3, SPEED3, 2., rand_spin, BRAKE3, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(4) + 5;
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE4, SPEED4, 2., rand_spin, BRAKE4, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(4) + 5;
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE5, SPEED5, 2., rand_spin, BRAKE5, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(4) + 5;
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE6, SPEED6, 2., rand_spin, BRAKE6, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;	// 回転値をランダムに取得 (-0.10 ~ 0.1)
		rand_type = GetRand(4) + 5;
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE7, SPEED7, 2., rand_spin, BRAKE7, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(4) + 5;
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE8, SPEED8, 2., rand_spin, BRAKE8, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(4) + 5;
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE9, SPEED9, 2., rand_spin, BRAKE9, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(4) + 5;
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE10, SPEED10, 2., rand_spin, BRAKE10, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(4) + 5;
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE11, SPEED11, 2., rand_spin, BRAKE11, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(4) + 5;
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE12, SPEED12, 2., rand_spin, BRAKE12, 255, 255, 255);
		break;
	case eSpread_Bigs:
		rand_spin = ((double)GetRand(20) - 10.) / 100.;	// 回転値をランダムに取得 (-0.10 ~ 0.1)
		piece[8]->Play(PosX, PosY, ANGLE + ANGLE1, SPEED1, 2., rand_spin, BRAKE1, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[8]->Play(PosX, PosY, ANGLE + ANGLE2, SPEED2, 2., rand_spin, BRAKE2, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[8]->Play(PosX, PosY, ANGLE + ANGLE3, SPEED3, 2., rand_spin, BRAKE3, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[8]->Play(PosX, PosY, ANGLE + ANGLE4, SPEED4, 2., rand_spin, BRAKE4, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[8]->Play(PosX, PosY, ANGLE + ANGLE5, SPEED5, 2., rand_spin, BRAKE5, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[8]->Play(PosX, PosY, ANGLE + ANGLE6, SPEED6, 2., rand_spin, BRAKE6, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;	// 回転値をランダムに取得 (-0.10 ~ 0.1)
		piece[8]->Play(PosX, PosY, ANGLE + ANGLE7, SPEED7, 2., rand_spin, BRAKE7, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[8]->Play(PosX, PosY, ANGLE + ANGLE8, SPEED8, 2., rand_spin, BRAKE8, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[8]->Play(PosX, PosY, ANGLE + ANGLE9, SPEED9, 2., rand_spin, BRAKE9, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[8]->Play(PosX, PosY, ANGLE + ANGLE10, SPEED10, 2., rand_spin, BRAKE10, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[8]->Play(PosX, PosY, ANGLE + ANGLE11, SPEED11, 2., rand_spin, BRAKE11, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		piece[8]->Play(PosX, PosY, ANGLE + ANGLE12, SPEED12, 2., rand_spin, BRAKE12, 255, 255, 255);
		break;
	case eSpread_All:
		rand_spin = ((double)GetRand(20) - 10.) / 100.;	// 回転値をランダムに取得 (-0.10 ~ 0.1)
		rand_type = GetRand(9);
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE1, SPEED1, 2., rand_spin, BRAKE1, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(9);
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE2, SPEED2, 2., rand_spin, BRAKE2, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(9);
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE3, SPEED3, 2., rand_spin, BRAKE3, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(9);
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE4, SPEED4, 2., rand_spin, BRAKE4, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(9);
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE5, SPEED5, 2., rand_spin, BRAKE5, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(9);
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE6, SPEED6, 2., rand_spin, BRAKE6, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;	// 回転値をランダムに取得 (-0.10 ~ 0.1)
		rand_type = GetRand(9);
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE7, SPEED7, 2., rand_spin, BRAKE7, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(9);
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE8, SPEED8, 2., rand_spin, BRAKE8, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(9);
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE9, SPEED9, 2., rand_spin, BRAKE9, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(9);
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE10, SPEED10, 2., rand_spin, BRAKE10, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(9);
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE11, SPEED11, 2., rand_spin, BRAKE11, 255, 255, 255);
		rand_spin = ((double)GetRand(20) - 10.) / 100.;
		rand_type = GetRand(9);
		piece[rand_type]->Play(PosX, PosY, ANGLE + ANGLE12, SPEED12, 2., rand_spin, BRAKE12, 255, 255, 255);
		break;
	}
}


void PieceEffect::GetRadian(double* rad, const double & ANGLE){
	*rad = ANGLE * DX_PI / 180.;	// 度数to弧度法
}

