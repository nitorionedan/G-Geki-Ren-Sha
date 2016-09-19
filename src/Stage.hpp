#pragma once

#include "Counter.hpp"
#include "Graphics2D.hpp"
#include "Vector2D.hpp"
#include "Graphic.hpp"
#include <DxLib.h>
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
	struct t_Camera
	{
		VECTOR pos;
		float vang;
		float hang;
		float tang;
	};

public:
	Stage();
	~Stage();
	void Finalize();
	void Update();
	void Draw();
	void StageSet(eStage estage);
	void NextStage();				// リザルト表示後呼ばれる
	void Clear    ();				// ステージクリアしたとき呼ばれる
	void AllClear ();				// 全クリ用
	void PlayQuake();
	void SkipTo(int Time);
	void Fadein();
	void Fadeout();
	const eStage GetNowStage() const {
		return nowStage;
	}
	const int GetTime() const {
		return time;
	}
	const int GetRank() const {
		return rank;
	}
	

private:
	void DrawStageCall();
	void Quake(); // ステージを揺らす

	std::unique_ptr<Graphic> graphic;
	std::unique_ptr<Counter> c_quake;	// ステージ全体を揺らす
	Field* mField;
	Vector2D pos;
	t_Camera tCamera;
	eStage	nowStage;					// 現在のステージ
	int    hs_bgm;						// BGM用ハンドル
	int    hs_boss;						// BGM用ハンドル
	int Screen;
	double cycle, shake;				// ラスタースクロール用 // [目標] 3.0, 400
	bool f_quake;						// シフトアップしたときの画面振動フラグ
	bool fadeinFlag;
	bool fadeoutFlag;
	bool isStanby;

	// static ------------
	int time;		// ステージの経過時間
	int rank;		// ランク
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
	static const int GetTime() {
		return mStage->GetTime();
	}
	static const int GetRank() {
		return mStage->GetRank();
	}

private:
	IStage() { mStage = nullptr; }

	static std::shared_ptr<Stage> mStage;
};