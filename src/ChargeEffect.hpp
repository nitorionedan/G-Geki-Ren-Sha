#pragma once

#include "EffectTask.hpp"
#include "Counter.hpp"
#include "Vector2D.hpp"

#include <memory>
#include <array>


class ChargeEffect : public EffectTask
{
private:
	static const int ENERGY_LEN = 8;

	const int ENERGY_DIS;	// 80 // 200
	const int ENERGY_EXR;	// 10
	const int ENERGY_TIM;	// 10

public:
	ChargeEffect();

	// @param[in]	dis_		エネルギーの出現範囲（半径）
	// @param[in]	exrate_		エネルギーの拡大率（exrate_ ～ exrate_ * 2までの範囲）
	// @param[in]	playTime	再生時間（フレーム）
	// @param[in]	moveTime	エネルギーの表示時間()
	ChargeEffect(int dis_, int exrate_, int playTime, int moveTime);
	~ChargeEffect();
	void Update() override;
	void Update(const double& PosX, const double& PosY) override;
	void Draw()	override;
	void PlayAnime(const double& PosX, const double& PosY) override;

private:
	// @brief		動き制御
	// @param[in]	i	添え字
	void Move(const int& i);

	// @brief		エネルギーのデータの初期化
	// @param[in]	i	添え字
	void Reset(const int& i);

	bool CheckFinish();

	// 全体の演出時間
	std::unique_ptr<Counter>	c_play;
	
	// 演出の中心座標
	Vector2D center;

	// エネルギーの座標
	std::array<Vector2D, ENERGY_LEN>	pos;

	// エネルギーの吸い込まれるまでの時間
	std::array<int, ENERGY_LEN>	move_time;

	// エネルギーの吸い込まれるまでの時間
	std::array<int, ENERGY_LEN>	move_Maxtime;

	// エネルギーと再生座標との最初の距離
	std::array<double, ENERGY_LEN>	dis;

	// エネルギーの拡大率
	std::array<double, ENERGY_LEN>	exrate;

	// エネルギーの拡大率
	std::array<double, ENERGY_LEN>	Minexrate;

	// エネルギーの飛ぶ吸い込まれる角度
	std::array<double, ENERGY_LEN>	angle;

	// エネルギーが吸い込まれたかどうか
	std::array<bool, ENERGY_LEN> isSweeped;

	double ene_rota;

	int gh_energy;

	int sh;

	// 演出しているか？
	bool isPlay;

	static int count;
};