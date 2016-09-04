#pragma once

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


class Stage	: public Graphics2D
{
public:
	Stage();
	~Stage();
	void Update();
	void Draw();
	void StageSet(eStage estage);
	void NextStage();								// ���U���g�\����Ă΂��
	void Clear    ();								// �X�e�[�W�N���A�����Ƃ��Ă΂��
	void AllClear ();								// �S�N���p
	void PlayQuake();
	int WrapPos  (int val, int max, int min);	// ���b�v�A���E���h
	const eStage GetNowStage() const {
		return nowStage;
	}
	static int GetTime();

private:
	void Move();
	void Quake(); // �X�e�[�W��h�炷

	static const double SPEED_BACK;
	static const double SPEED_MIDDLE;
	static const double SPEED_FRONT;

	std::unique_ptr<Counter> c_quake;								// �X�e�[�W�S�̂�h�炷
	std::array<int, 3> hg;											// �摜�p�n���h��
	std::array<Vector2D, 2> stage_back, stage_mid, stage_fro;		// �i�O�E�^���E��j�w�i���ꂼ��̍��W

	eStage	nowStage;												// ���݂̃X�e�[�W
	int    hs_bgm;													// BGM�p�n���h��
	int    hs_boss;													// BGM�p�n���h��
	int GaussScreen;
	double cycle, shake;											// ���X�^�[�X�N���[���p // [�ڕW] 3.0, 400
	bool   f_quake;													// �V�t�g�A�b�v�����Ƃ��̉�ʐU���t���O

	// static ------------
	static int s_time;		// �X�e�[�W�̌o�ߎ���
	static int s_rank;		// �����N
};


/*
@brief		Stage�N���X�̃C���^�[�t�F�C�X
@attention	Stage�N���X�ɃA�N�Z�X�������ꍇ�͂�����ɃA�N�Z�X����
@warning	�ŏ��Ɉ�x����set�֐����Ă΂Ȃ���Ȃ����Ȃ�
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
	static const eStage GetNowStage() {
		return mStage->GetNowStage();
	}

private:
	IStage() { mStage = nullptr; }

	static std::shared_ptr<Stage> mStage;
};