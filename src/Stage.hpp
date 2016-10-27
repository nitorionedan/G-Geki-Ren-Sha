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
	void NextStage();				// ���U���g�\����Ă΂��
	void Clear    ();				// �X�e�[�W�N���A�����Ƃ��Ă΂��
	void AllClear ();				// �S�N���p
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
	void Quake(); // �X�e�[�W��h�炷
	void SmallQuale();
	void BigQuake();

	std::unique_ptr<Graphic> graphic;
	std::unique_ptr<Counter> c_quake;	// �X�e�[�W�S�̂�h�炷
	std::unique_ptr<Distortion> dist;	// ��ʂɘc�݂�^����
	Field* mField;
	Vector2D pos;
	t_Camera tCamera;
	eStage	nowStage;					// ���݂̃X�e�[�W
	eState state;						// ���
	eQuake quakeType;
	int Screen, gh_black;
	int time;							// �X�e�[�W�̌o�ߎ���
	int rank;							// �����N
	int c_bossBgm;
	int c_fade;
	bool f_quake;						// �V�t�g�A�b�v�����Ƃ��̉�ʐU���t���O
	bool isStanby;
	int start_score;
	int end_score;
	int sum_score;
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