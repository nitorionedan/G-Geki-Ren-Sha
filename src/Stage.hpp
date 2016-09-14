#pragma once

#include "Counter.hpp"
#include "Graphics2D.hpp"
#include "Vector2D.hpp"
#include <memory>
#include <string>


class Field;

enum class eStage
{
	opening,
	stage1,
	stage2,
	stage3,
	stage4,
	stage5,
	stage6,
	stage0,
};


class Stage	: public Graphics2D
{
public:
	Stage();
	~Stage();
	void Finalize();
	void Update();
	void Draw();
	void StageSet(eStage estage);
	void NextStage();								// リザルト表示後呼ばれる
	void Clear    ();								// ステージクリアしたとき呼ばれる
	void AllClear ();								// 全クリ用
	void PlayQuake();
	void SkipTo(int Time);
	const eStage GetNowStage() const {
		return nowStage;
	}
	static const int GetTime() {
		return s_time;
	}
	

private:
	void Quake(); // ステージを揺らす

	std::unique_ptr<Counter> c_quake;								// ステージ全体を揺らす
	Field* mField;
	Vector2D pos;
	eStage	nowStage;												// 現在のステージ
	int    hs_bgm;													// BGM用ハンドル
	int    hs_boss;													// BGM用ハンドル
	int Screen;
	double cycle, shake;											// ラスタースクロール用 // [目標] 3.0, 400
	bool   f_quake;													// シフトアップしたときの画面振動フラグ

	// static ------------
	static int s_time;		// ステージの経過時間
	static int s_rank;		// ランク
};


/*
@brief		Stageクラスのインターフェイス
@attention	Stageクラスにアクセスしたい場合はこちらにアクセスする
@warning	最初に一度だけset関数を呼ばなけれないけない
*/
class IStage
{
public:
	~IStage() {}
	static void set(std::shared_ptr<Stage>);
	static void Load();
	static void Quake() {
		mStage->PlayQuake();
	}
	static void SkipTo(int Time);
	static const eStage GetNowStage() {
		return mStage->GetNowStage();
	}

private:
	IStage() { mStage = nullptr; }

	static std::shared_ptr<Stage> mStage;
};