//------------------------------------------------------------------------------
/// @file
/// @brief    Pieceクラス
/// @author   PG班 矢風！
///
/// @copyright  Copyright 2016 Shohei.Y
/// @attention  このファイルの利用は、同梱のREADMEにある
///             利用条件に従ってください

//------------------------------------------------------------------------------

#pragma once

#include "EffectTask.hpp"
#include "Vector2D.hpp"
#include "Counter.hpp"

#include <array>


enum ePiece_t
{
	ePiece_SmallGray,	// 小さい破片（灰色）
	ePiece_SmallCoin,	// コインみたいな
	ePiece_SmallSquare,	// 正方形
	ePiece_Smalls,		// 粉々（８種類）
	ePiece_BigNut,		// ナット
	ePiece_BigPlate,	// 板
	ePiece_BigSquare,	// 正方形
	ePiece_BigPlate3,
	ePiece_Bigs,		// 大きな破片たち
	ePiece_Special,		// ハンバーガー、ロゴ

	ePiece_None
};


class Piece : public EffectTask
{
private:
	static const int PIECE_ALL_NUM = 60;

public:
	Piece() : type(ePiece_None){}
	Piece(ePiece_t type_);
	~Piece();
	void Update() override;
	void Draw() override;

	// @brief		破片を１つ飛ばす
	// @param[in]	PosX, PosY	座標
	// @param[in]	ANGLE	飛ばす角度（度数法）
	// @param[in]	VEC		飛ばす力
	// @param[in]	FSPEED	コマ送りの速さ
	// @param[in]	SPIN	回転する速さ
	// @param[in]	TYPE	破片の種類
	// @param[in]	R,G,B	RGB値。0でグレースケール
	// @note		８個、falseであるisPlayを抽出しその添え字に対応するメンバ変数に引数の値を設定する
	// @attention	falseであるisPlayが無い場合、すかす
	void Play(const double& PosX, const double& PosY, const double& V_ANGLE, const double& VEC, const int& FSPEED, const double& SPIN, const double& BRAKE, int R, int G, int B);

private:
	void PlayAnime(const double& PosX, const double& PosY) override;
	
	// @brief		Drawの中身の分割
	// @param[in]	i	添え字の番号
	void Draw_Piece(const int& i);

	// @brief		破片の動きを制御
	// @param[in]	INDEX	添え字の番号
	// @attention	0からALL_NUMの値まで
	void Move(const int& INDEX);

	const ePiece_t type;

	std::array<Vector2D, PIECE_ALL_NUM> pos;		// 座標
	std::array<int, PIECE_ALL_NUM> fspeed;		// アニメーション速度
	std::array<int, PIECE_ALL_NUM> red, green, blue;		// RGB値
	std::array<int, PIECE_ALL_NUM> rand;			// ランダム種類
	std::array<double, PIECE_ALL_NUM> vangle;		// 飛ばす角度
	std::array<double, PIECE_ALL_NUM> vec;		// 飛ばす力
	std::array<double, PIECE_ALL_NUM> angle;		// 角度
	std::array<double, PIECE_ALL_NUM> spin;		// 回転速度
	std::array<double, PIECE_ALL_NUM> brake;		// ブレーキ
	std::array<bool, PIECE_ALL_NUM> isPlay;		// 再生中か？
	std::array<bool, PIECE_ALL_NUM> isSlow;		// 減速中か？
	Counter** c_play;						// 再生させるフレーム数カウンター

	int gh_small1[8],	// 灰
		gh_small5[4],	// 小さい正方形
		gh_small6[5],	// コインみたいな
		gh_smalls[8],	// 粉々
		gh_big1[8],		// ナット
		gh_big2[14],	// 長方形１
		gh_big3[6],		// 正方形
		gh_big5[14],		// 大きな破片
		gh_bigs[8],		// 大きな破片たち
		gh_kit,
		gh_burger;
};