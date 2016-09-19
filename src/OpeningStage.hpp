#pragma once

#include "Field.hpp"
#include "Vector2D.hpp"
#include <memory>
#include <array>

class Counter;

enum eBG
{
	eBG_back,
	eBG_middle,
	eBG_front,
};


class OpenigStage : public Field
{
public:
	OpenigStage();
	~OpenigStage();
	void Initialize();
	virtual void Update() override;
	virtual void Draw() override;

private:
	int WrapPos(int val, int max, int min);	// ラップアラウンド

	const double SPEED_BACK;
	const double SPEED_MIDDLE;
	const double SPEED_FRONT;

	std::array<int, 3> hg;											// 画像用ハンドル
	std::array<Vector2D, 2> stage_back, stage_mid, stage_fro;		// （前・真中・後）背景それぞれの座標

	Vector2D pos;
	int elapsedTime;
	int Screen;
	int c_trans;
	double cycle, shake; // raster scroll
};