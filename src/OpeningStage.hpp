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
	int WrapPos(int val, int max, int min);	// ���b�v�A���E���h

	const double SPEED_BACK;
	const double SPEED_MIDDLE;
	const double SPEED_FRONT;

	std::array<int, 3> hg;											// �摜�p�n���h��
	std::array<Vector2D, 2> stage_back, stage_mid, stage_fro;		// �i�O�E�^���E��j�w�i���ꂼ��̍��W

	Vector2D pos;
	int elapsedTime;
	int Screen;
	int c_trans;
	double cycle, shake; // raster scroll
};