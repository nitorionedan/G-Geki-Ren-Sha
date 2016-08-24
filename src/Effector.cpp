/*


まったく使い物にならない失敗作


*/


#include "DxLib.h"

#include "Effector.hpp"
//#include <new>


const int	Effector::MAX_HIT_NUM = 20;

Vector2D	Effector::s_hitPos[MAX_HIT_NUM];
bool		Effector::s_isHitAnime[MAX_HIT_NUM];
int			Effector::s_hg_hit[4];


Effector::Effector()
	: c_anime(new Counter(60))
	, f_up(false)
	, f_down(false)
	, f_PlayAnime(false)
	, shiftLv(0)
{
	for (auto &c : c_hit)	c = new Counter(16);

	// 画像
	hg_shift.at(0) = LoadGraph("GRAPH/GAME/EFFECT/shift_0.png");
	hg_shift.at(1) = LoadGraph("GRAPH/GAME/EFFECT/shift_1.png");
	hg_shift.at(2) = LoadGraph("GRAPH/GAME/EFFECT/shift_2.png");
	hg_shift.at(3) = LoadGraph("GRAPH/GAME/EFFECT/shift_3.png");
	hg_shift.at(4) = LoadGraph("GRAPH/GAME/EFFECT/shift_4.png");
	LoadDivGraph("GRAPH/GAME/EFFECT/hit128x32.png", 4, 4, 1, 128, 32, s_hg_hit);

	// 音
	hs_shift.at(0) = LoadSoundMem("SOUND/SE/shiftup.mp3");
	hs_shift.at(1) = LoadSoundMem("SOUND/SE/shiftdown.wav");

	// static
	Vector2D temp;
	temp.SetVecor2D(0.0, 0.0);
	for (int i = 0; i < MAX_HIT_NUM; ++i)
	{
		s_hitPos[i] = temp;
		s_isHitAnime[i] = false;
	}
}


Effector::~Effector()
{
	for (auto &c : c_hit) delete c;
	for (int &Gr_Handle : hg_shift)	DeleteGraph(Gr_Handle);
	for (int &hg : s_hg_hit)	DeleteGraph(hg);
	for (int &So_Handle : hs_shift)	DeleteSoundMem(So_Handle);
}


void Effector::Update(){
	if(f_PlayAnime) c_anime->Update();
}


void Effector::Draw()
{
	if (!f_PlayAnime)	return;

	// ここにアニメーションを書いていく
	if(f_up)
	{
		SetDrawBright(255, 0, 0);	// 赤色に設定
		DrawRotaGraph(320, 240, 2.0, 0.0, hg_shift.at(shiftLv), true);
	}
	if(f_down)
	{
		SetDrawBright(0, 0, 255);	// 青色に設定
		DrawRotaGraph(320, 240, 2.0, 0.0, hg_shift.at(shiftLv), true);
	}

	// 指定の時間がくると消える
	if(c_anime->isLast())
	{
		f_PlayAnime = false;
		c_anime->Reset();
	}

	SetDrawBright(255, 255, 255);	// 元の色合い
}


void Effector::Shift(const bool isSHIFT_UP, const int pow)
{
	// SHIFT UP
	if (isSHIFT_UP)		ShiftUp(pow);

	// SHIFT DOWN
	if (!isSHIFT_UP)	ShiftDown(pow);

	f_PlayAnime = true; // アニメーション開始
}


void Effector::Update_HitAnime() // Game.cppに置く
{

}


void Effector::Draw_HitAnime() // Game.cppに置く
{

}


bool Effector::getIsAnime(){
	return f_PlayAnime;
}


void Effector::PlayHitAni(const double& PosX, const double& PosY)
{
}


void Effector::ShiftUp(const int powlv)
{
	if (f_down)	f_down = false;
	f_up = true;
	shiftLv = powlv;
}


void Effector::ShiftDown(const int powlv)
{
	if (f_up) f_up = false;
	f_down = true;
	shiftLv = powlv;
}


void Effector::CopyCounter()
{

}
