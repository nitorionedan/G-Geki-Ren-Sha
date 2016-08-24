//------------------------------------------------------------------------------
/// @file
/// @brief    BossCharaクラス
/// @author   PG班 矢風！
///
/// @copyright  Copyright 2016 Shohei.Y
/// @attention  このファイルの利用は、同梱のREADMEにある
///             利用条件に従ってください

//------------------------------------------------------------------------------

#pragma once

#include "BossTask.hpp"


enum eBoss_num
{
	eBoss_A,
	eBoss_B,
	eBoss_C,
	eBoss_D,
	eBoss_E,
	eBoss_F,
	eBoss_G,

	eBoss_None,
};


class BossChara
{
public:
	BossChara(BossTask* mBoss_);
	~BossChara();
	void Update();
	void Draw();

	// @brief		指定座標との当たり判定
	// @param[in]	ColX, ColY	衝突する物体の座標
	// @param[in]	DamagePoint	ダメージ値
	bool HitCheck(const double& ColX, const double& ColY, const int& DamagePoint);
	
	// @brief		ボスを登場させる
	// @param[in]	登場させるボスの種類
	void Start(eBoss_num num);

private:
	BossChara(){}
	BossTask* mBoss;
};
