#pragma once

#include "Keyboard.hpp"
#include "Graphic.hpp"
#include "Range.hpp"
#include "Counter.hpp"
#include "Vector2D.hpp"
#include "PieceEffect.hpp"
#include "Bomb.hpp"
#include <memory>


// ���͂����L�[�̕���
enum class eInputDir
{
	Neutral,
	Right,
	Left
};


enum class ePlayerState
{
	Start,
	Game,
	Dead,
};


class Player
{
public:
	Player();
	~Player();
	void Update();
	void Draw();
	void setup(std::shared_ptr<Bomb>);
	void Update_Start();
	void Update_Game();
	void Update_Dead();
	void Draw_Start();
	void Draw_Game();
	void Draw_Dead();
	void SetStart();
	void InputMng();	// �A�˃f�[�^�Ǘ�
	void GetPos(double* x, double* y) {
		*x = this->pos.x;
		*y = this->pos.y;
	}
	const Vector2D& GetPos() const {
		return pos;
	}
	void AddBomb() {
		bombNum++;
	}
	void ShiftReset() {
		powlv = 0;
	}
	void DownBombNum();
	
	// @brief	�A�[�����Z�b�g����
	// @attention	�A�[���������Ȃ瓾�_
	void SetArm();

	void Shift(const bool isUp);
	const int GetShiftLevel() const {
		return powlv;
	}
	const int GetBombNum() const {
		return bombNum;
	}
	const int GetLife() const {
		return life;
	}
	const int GetRensha() const {
		return rensha;
	}
	const bool IsDead() const {
		return isDead;
	}
	const bool IsMuteki() const {
		return isMuteki;
	}
	const bool IsStart() const {
		return isStart;
	}

	// @brief	���@�̉~�Ƒ��̓_�̓����蔻��
	bool HitCheckCircle(const double& ColX, const double& ColY);

	// @brief	���@�̉~�Ƒ��̉~�̓����蔻��
	bool HitCheckCircle(const double & Range1, const double & Range2, const double & X1, const double & Y1, const double & X2, const double & Y2);

	static const int HIT_RANGE;
	
private:
	void Move();			// �v���C���[�̓���
	void Rensha_Update();

	static int const MaxPowLevel;				// �V�t�g�̌��E�l
	static int const MAX_RENSHA;				// �A�˃Q�[�W�͈̔�
	static int const MIN_RENSHA;
	double const SpeedForce;					// �ړ����x
	double const LimitScreenXL, LimitScreenXR;	// ��ʐ���
	double const LimitScreenYT, LimitScreenYB;
	double const Y_START;						// �X�^�[�g�n�_

	int	rensha;		// �A�˃Q�[�W
	bool isStart;		// �X�^�[�g�n�_�ɓ��B�������H
	bool isHit;			// ��e�������H
	bool isArm;			// �A�[���W�J�����H
	// ---------------------------

	std::unique_ptr<Graphic> graphic;	// �����摜
	std::unique_ptr<Counter> c_start;	// �X�^�[�g�����p�J�E���^�[
	std::unique_ptr<Counter> c_dead;
	std::shared_ptr<Bomb> mBomber;

	eInputDir keydir;		// �L�[���͕���
	ePlayerState state;		// �v���C���[�̏��
	eSpread_t dead_ef;		// ���ꉉ�o
	Vector2D pos;			// ���@�̍��W
	Vector2D vec;			// ���@�̈ړ��x�N�g��
	int	hs_shiftUp,			// ���n���h��
		hs_shiftDown,
		hs_dead;
	int	bombNum;			// ���݂̃{����
	int	hg[3];				// �摜�p�n���h��
	int	hg_arm[12];			// �V�[���h�摜�n���h��
	int	life;				// �c�@
	int	elapsedTime;		// �o�ߎ���
	int powlv;				// �p���[���x��(�V�t�g���x��)
	bool isDead;			// ����ł��邩�H
	bool isMuteki;			// ���G���H
};