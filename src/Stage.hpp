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

public:
	Stage();
	~Stage();
	void Initialize();
	void Finalize();
	void Update();
	void Draw();
	void UpdateField();
	void StageSet(eStage estage);
	void NextStage();				// ���U���g�\����Ă΂��
	void Clear    ();				// �X�e�[�W�N���A�����Ƃ��Ă΂��
	void AllClear ();				// �S�N���p
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
	void Update_Result();
	void Update_Fadeout();
	void DrawStageCall();
	void DrawResult();
	void Quake(); // �X�e�[�W��h�炷

	std::unique_ptr<Graphic> graphic;
	std::unique_ptr<Counter> c_quake;	// �X�e�[�W�S�̂�h�炷
	Field* mField;
	Vector2D pos;
	t_Camera tCamera;
	eStage	nowStage;					// ���݂̃X�e�[�W
	eState state;						// ���
	int Screen;
	int time;							// �X�e�[�W�̌o�ߎ���
	int rank;							// �����N
	double soundVolume;
	bool f_quake;						// �V�t�g�A�b�v�����Ƃ��̉�ʐU���t���O
	bool fadeinFlag;
	bool fadeoutFlag;
	bool isStanby;
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
	static void reset() {
		mStage.reset();
	}
	static void Load();
	static void Quake() {
		mStage->PlayQuake();
	}
	void Clear() {
		mStage->Clear();
	}
	void AllClear() {
		mStage->AllClear();
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