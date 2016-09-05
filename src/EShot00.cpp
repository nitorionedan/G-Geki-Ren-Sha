#include "DxLib.h"

#include "EShot00.hpp"
#include "Graphics2D.hpp"
#include "Shot.hpp"
#include "HitEffect.hpp"
#include "Bomb.hpp"

#include <cmath>


EShot00::EShot00()
	: ALL_FRAME_NUM(sizeof(gh) / sizeof(gh[0]))
	, FRAME_TIME(2)
	, ALL_FRAME_TIME(ALL_FRAME_NUM * FRAME_TIME + 1) // �]������Ƃ߂邽�߁A�P�𑫂��Ē���
	, HIT_RANGE(7)
{
	time.fill(0);
	rad.fill(0.);
	isExist.fill(false);
	LoadDivGraph("GRAPH/GAME/Eshot/eshot00.png", 12, 4, 3, 8, 8, gh);
}


EShot00::~EShot00()
{
	for (int i = 0; i < _countof(gh); i++)
		DeleteGraph(gh[i]);
}


void EShot00::Update(const double& PosX, const double& PosY)
{
	for (int i = 0; i < ALL_NUM; i++)
	{
		if (!isExist[i])
		{
			pos[i].SetVec(PosX, PosY);	 continue;
		}
	
		time[i]++;
		Move(i);
	}
}


void EShot00::Draw()
{
	for (int i = 0; i < ALL_NUM; i++)
	{
		if (!isExist[i])	continue;

		// �A�j���[�V����
		DrawAnime(pos[i].x, pos[i].y, vangle[i], time[i], ALL_FRAME_NUM, FRAME_TIME, gh);
	}
}


void EShot00::Fire(const double& SPEED, const double& ANGLE)
{
	for (int i = 0; i < ALL_NUM; i++)
	{
		if (isExist[i])	continue;

		isExist[i] = true;
		vspeed[i] = SPEED;
		vangle[i] = ANGLE;	break;
	}
}


void EShot00::Fire(const double & PosX, const double & PosY, const double & SPEED, const double & ANGLE)
{
	for (int i = 0; i < ALL_NUM; i++)
	{
		if (isExist[i])	continue;

		isExist[i] = true;
		pos[i].SetVec(PosX, PosY);	 continue;
		vspeed[i] = SPEED;
		vangle[i] = ANGLE;	break;
	}
}


void EShot00::Move(const int & id)
{
	// �i�܂���
	pos[id].x += vspeed[id] * std::cos(vangle[id]);
	pos[id].y += vspeed[id] * std::sin(vangle[id]);

	// �����蔻��`�F�b�N
	const bool& IS_HIT = IPlayer::HitCheckCircl(HIT_RANGE, pos[id]);
	const bool& IS_HIT2 = IBomb::IsHit(HIT_RANGE, pos[id].x, pos[id].y);

	// �������������
	if (IS_HIT || IS_HIT2)
	{
		isExist[id] = false;
		IHitEffect::PlayAnime(pos[id]);
	}

	// ��ʊO�ŏ���
	if (Shot::SC_LIMIT_XL > pos[id].x ||
		Shot::SC_LIMIT_XR < pos[id].x ||
		Shot::SC_LIMIT_YT > pos[id].y ||
		Shot::SC_LIMIT_YB < pos[id].y)
	{
		isExist[id] = false;
	}
}


bool EShot00::IsHit(const double & ColX, const double & ColY, const double & ColR)
{
	const bool& IS_HIT = Vector2D::CirclesCollision(HIT_RANGE, ColR, pos[0].x, pos[0].y, ColX, ColY + 9.);

	return IS_HIT;
}

// EOF