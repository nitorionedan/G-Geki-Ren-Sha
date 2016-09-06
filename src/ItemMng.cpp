#include "ItemMng.hpp"
#include "Graphics2D.hpp"
#include "DebugMode.hpp"
#include "Game.hpp"
#include "Player.hpp"

#include <DxLib.h>
#include <cmath>
#include <cassert>


ItemMng::ItemMng()
	: LIMIT_L(10)
	, LIMIT_R(630.)
	, LIMIT_T(10.)
	, LIMIT_B(480.)
	, DELETE_TIME(780)
	, HIT_RANGE(20)
{
	LoadDivGraph("GRAPH/GAME/itemB.png", 32, 8, 4, 16, 14, gh_b);
	LoadDivGraph("GRAPH/GAME/itemP.png", 32, 8, 4, 16, 14, gh_p);
	LoadDivGraph("GRAPH/GAME/itemS.png", 32, 8, 4, 16, 14, gh_s);
	sh_get = LoadSoundMem("SOUND/SE/1up.mp3");

	time.fill(0);
	isExist.fill(false);
	isDeleting.fill(false);
	for (auto vec : dir)
		vec.SetVec(1., 1.);
}


ItemMng::~ItemMng()
{
	for (int i = 0; i < sizeof(gh_b) / sizeof(gh_b[0]); i++)	DeleteGraph(gh_b[i]);
	for (int i = 0; i < sizeof(gh_p) / sizeof(gh_p[0]); i++)	DeleteGraph(gh_p[i]);
	for (int i = 0; i < sizeof(gh_s) / sizeof(gh_s[0]); i++)	DeleteGraph(gh_s[i]);
	DeleteSoundMem(sh_get);
}


void ItemMng::Update(std::shared_ptr<Player> player)
{
	for (int i = 0; i < NUM; i++)
	{
		if (!isExist[i])	continue;

		time[i]++;

		if (time[i] == DELETE_TIME - 180)	isDeleting[i] = true;
		if (time[i] == DELETE_TIME)			isExist[i] = false;
	
		Move(i);
		HitCheck(i, player);
	}
}


void ItemMng::Draw()
{
	for (int i = 0; i < NUM; i++)
	{
		if (!isExist[i])	continue;

		if (isDeleting[i] && time[i] % 4 < 2)	continue;

		switch (type[i])
		{
		case Bomber:
			DrawAnime(pos[i].x, pos[i].y, 2., 0., time[i], sizeof(gh_b) / sizeof(gh_b[0]), 3, gh_b);	break;
		case Power:
			DrawAnime(pos[i].x, pos[i].y, 2., 0., time[i], sizeof(gh_p) / sizeof(gh_p[0]), 3, gh_p);	break;
		case Shield:
			DrawAnime(pos[i].x, pos[i].y, 2., 0., time[i], sizeof(gh_s) / sizeof(gh_s[0]), 3, gh_s);	break;

		default:
			break;
		}
	}


	if (!DebugMode::isTest)	return;

}


void ItemMng::Create(double PosX, double PosY)
{
	for (int i = 0; i < NUM; i++)
	{
		if (isExist[i])	continue;

		Reset(PosX, PosY, i);

		// �A�C�e���̎�ނ����߂�
		type[i] = static_cast<eItem>( GetRand(2) );

		break;
	}
}


void ItemMng::Create(double PosX, double PosY, eItem type)
{
	for (int i = 0; i < NUM; i++)
	{
		if (isExist[i])	continue;

		Reset(PosX, PosY, i);

		// �A�C�e���̎�ނ����߂�
		this->type[i] = type;

		break;
	}
}


void ItemMng::Move(const int & id)
{
	pos[id].x += std::cos(vang[id]) * vel[id].x;
	pos[id].y += std::sin(vang[id]) * vel[id].y;

	// ���˕Ԃ�
	if (pos[id].x < LIMIT_L || pos[id].x > LIMIT_R)
		vel[id].x *= -1.;

	if (pos[id].y < LIMIT_T)	vel[id].y *= -1.;

	// �ޏ�
	if (pos[id].y > LIMIT_B)
	{
		isExist[id] = false;
		time[id] = 0;
	}
}


void ItemMng::HitCheck(const int& id, std::shared_ptr<Player> player)
{
	const bool& IS_HIT = Vector2D::CirclesCollision(8, HIT_RANGE, player->GetPos().x, player->GetPos().y, pos[id].x, pos[id].y);

	if(IS_HIT)
	{
		switch (type[id])
		{
		case Bomber:
			player->AddBomb();
			PlaySoundMem(sh_get, DX_PLAYTYPE_BACK);
			break;
		case Power:
			player->Shift(true);
			PlaySoundMem(sh_get, DX_PLAYTYPE_BACK);
			break;
		case Shield:
			player->SetArm();
			PlaySoundMem(sh_get, DX_PLAYTYPE_BACK);
			break;

		default:
			break;
		}

		isExist[id] = false;
	}
}


void ItemMng::Reset(double PosX, double PosY, int id)
{
	isExist[id] = true;
	isDeleting[id] = false;
	time[id] = 0;
	pos[id].SetVec(PosX, PosY);
	vel[id].x = GetRand(1) + 2.;
	vel[id].y = GetRand(1) + 2.;
//	vang[id] = std::cos(GetRand(100));
	vang[id] = GetRand(200) - 100;
}



// =======================================================
std::shared_ptr<ItemMng> IItemMng::mItemMng;


void IItemMng::set(std::shared_ptr<ItemMng> itemMng)
{
	mItemMng = itemMng;
	assert(mItemMng != nullptr && "IItemMng::set()");
}