#pragma once

#include "Counter.hpp"
#include "Vector2D.hpp"
#include "Graphic.hpp"
#include <DxLib.h>
#include <memory>
#include <string>
#include <vector>

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


enum class eQuake
{
	smal,
	normal,
	big,
};


class Stage
{
	enum class eState
	{
		game,
		result,
	};

	struct t_Camera
	{
		VECTOR pos;
		float vang;
		float hang;
		float tang;
	};

	class Distortion
	{
		struct tDist
		{
			Vector2D pos;
			double range;
			int time, sc;
			bool isPlay;
			bool operator==(const tDist& other) {
				return (pos == other.pos) &&
					(range == other.range) &&
					(time == other.time) &&
					(isPlay == other.isPlay);
			}
		};

	public:
		Distortion();
		~Distortion();
		void Update();
		void Draw(const int& GrHandle);
		void PlayDistortion(double x, double y);
	
	private:
		std::vector<tDist> m_dist;
		int screen;
	};

public:
	Stage();
	~Stage();
	void Initialize();
	void Finalize();
	void Update();
	void Draw();
	void UpdateField();
	void StageSet(eStage estage);
	void NextStage();				// リザルト表示後呼ばれる
	void Clear    ();				// ステージクリアしたとき呼ばれる
	void AllClear ();				// 全クリ用
	void PlayQuake();
	void PlayQuake(eQuake aukeType);
	void PlayDistortion(double x, double y);
	void SkipTo(int Time);
	const eStage GetNowStage() const {
		return nowStage;
	}
	const int GetTime() const {
		return time;
	}
	const int GetRank() const {
		return rank;
	}
	static bool s_isContinue;

private:
	void Update_Result();
	void DrawStageCall();
	void DrawResult();
	void Quake(); // ステージを揺らす
	void SmallQuale();
	void BigQuake();

	std::unique_ptr<Graphic> graphic;
	std::unique_ptr<Counter> c_quake;	// ステージ全体を揺らす
	std::unique_ptr<Distortion> dist;	// 画面に歪みを与える
	Field* mField;
	Vector2D pos;
	t_Camera tCamera;
	eStage	nowStage;					// 現在のステージ
	eState state;						// 状態
	eQuake quakeType;
	int Screen, gh_black;
	int time;							// ステージの経過時間
	int rank;							// ランク
	int c_bossBgm;
	int c_fade;
	bool f_quake;						// シフトアップしたときの画面振動フラグ
	bool isStanby;
	int start_score;
	int end_score;
	int sum_score;
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
	static void reset() {
		mStage.reset();
	}
	static void Load();

	static void Quake(eQuake auakeType);
	
	static void Clear() {
		mStage->Clear();
	}
	static void AllClear() {
		mStage->AllClear();
	}
	static void SkipTo(int Time);
	static const eStage GetNowStage() {
		return mStage->GetNowStage();
	}
	static void PlayDist(double x, double y) {
		mStage->PlayDistortion(x, y);
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