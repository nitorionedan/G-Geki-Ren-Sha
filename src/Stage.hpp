#pragma once

#include "Task.hpp"
#include "Counter.hpp"
#include "Graphics2D.hpp"
#include "Vector2D.hpp"

#include <memory>
#include <array>


enum class eStage
{
	stage1,
	stage2,
	stage3,
	stage4,
	stage5,
	stage6,
	stage0,
};


enum eBG
{
	eBG_back,
	eBG_middle,
	eBG_front,
};


class Stage
	: public Task
	, public Graphics2D
{
public:
	Stage();
	~Stage();
	virtual void Update() override;
	virtual void Draw() override;
	void NextStage();								// リザルト表示後呼ばれる
	void Clear    ();								// ステージクリアしたとき呼ばれる
	void AllClear ();								// 全クリ用
	void PlayQuake();
	int WrapPos  (int val, int max, int min);	// ラップアラウンド

	static int GetTime();

	eStage	nowStage;							// 現在のステージ

private:
	void StageSet(eStage estage);
	void Move();
	void Quake();								// ぶるぶる


	static const double SPEED_BACK;
	static const double SPEED_MIDDLE;
	static const double SPEED_FRONT;

	std::unique_ptr<Counter> c_quake;								// ステージ全体を揺らす
	std::array<int, 3> hg;											// 画像用ハンドル
	std::array<Vector2D, 2> stage_back, stage_mid, stage_fro;		// （前・真中・後）背景それぞれの座標

	int    hs_bgm;													// BGM用ハンドル
	int    hs_boss;													// BGM用ハンドル
	double cycle, shake;											// ラスタースクロール用 // [目標] 3.0, 400
	bool   f_quake;													// シフトアップしたときの画面振動フラグ

	// static ------------
	static int s_time;		// ステージの経過時間
	static int s_rank;		// ランク
};