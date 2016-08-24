#pragma once

#include "Task.hpp"
#include "Keyboard.hpp"
#include "Graphic.hpp"
#include "Range.hpp"
#include "Counter.hpp"
#include "Enemy.hpp"
#include "Vector2D.hpp"
#include "PieceEffect.hpp"

#include <memory>


// 入力したキーの方向
enum class InputDir
{
	Neutral,
	Right,
	Left
};


enum ePlayerState
{
	ePlayerState_Start,
	ePlayerState_Game,
	ePlayerState_Dead,
};


class Player : public Task
{
public:
	Player();
	~Player();
	void Update()	override;
	void Draw()		override;
	void Update_Start();
	void Update_Game();
	void Update_Dead();
	void Draw_Start();
	void Draw_Game();
	void Draw_Dead();
	void SetStart();
	void InputMng();						// 連射データ管理
	void GetPos(double* x, double* y);
	Vector2D& GetPos();
	void AddBomb();
	void ShiftReset();
	void DownBombNum();
	
	// @brief	アームをセットする
	// @attention	アーム装備中なら得点
	static void SetArm();

	// 座標を外部へ伝えるため-----
	static void Shift(const bool isUp);			// シフト操作
	static int GetShiftLevel();
	static int GetBombNum();
	static int GetLife();
	static int GetRensha();
	static bool IsDead();
	static bool IsMuteki();
	static bool IsStart();

	// @brief	自機の円と他の点の当たり判定
	bool HitCheckCircle(const double& ColX, const double& ColY);

	// @brief	自機の円と他の円の当たり判定
	bool HitCheckCircle(const double & Range1, const double & Range2, const double & X1, const double & Y1, const double & X2, const double & Y2);

	static const int HIT_RANGE;
	

private:
	void Move();									// プレイヤーの動き
	void CopyStaticMem();							// staticメンバ変数に値をコピー
	void Rensha_Update();

	static const int	MaxPowLevel;				// シフトの限界値
	static const int	MAX_RENSHA,					// 連射ゲージの範囲
						MIN_RENSHA;
	const double	SpeedForce;						// 移動速度
	const double	LimitScreenXL, LimitScreenXR;	// 画面制限
	const double	LimitScreenYT, LimitScreenYB;
	const double	Y_START;						// スタート地点

	// 座標を外部へ伝えるため-----
	static ePlayerState state;		// プレイヤーの状態
	static Vector2D s_pos;
	static int	hs_shiftUp,
				hs_shiftDown,
				hs_dead;
	static int	s_powlv;		// パワーレベル(シフトレベル)
	static int	s_bombNum;		// ボム所持数
	static int	bombNum;		// 現在のボム数
	static int	s_life;			// 残機
	static int	s_rensha;		// 連射ゲージ
	static bool	s_isDead;
	static bool	s_isMuteki;
	static bool	s_isStart;		// スタート地点に到達したか？
	static bool	isHit;			// 被弾したか？
	static bool isArm;			// アーム展開中か？
	// ---------------------------

	std::unique_ptr<Graphic>	graphic;	// 文字画像
	std::unique_ptr<Counter>	c_start;	// スタート準備用カウンター
	std::unique_ptr<Counter>	c_dead;
	std::unique_ptr<Enemy>		test;		// テスト用サンドバッグ

	InputDir	 keydir;	// キー入力方向
	eSpread_t	dead_ef;	// やられ演出
	Vector2D	pos;		// 自機の座標
	static Vector2D	vec;	// 自機の移動ベクトル
	int			hg[3];		// 画像用ハンドル
	int			hg_arm[12];	// シールド画像ハンドル
	int			life;		// 残機
	int			elapsedTime;
	bool		isDead;		// 死んでいるか？
	bool		isMuteki;	// 無敵か？
};
// EOF