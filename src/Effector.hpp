#pragma once

#include "Counter.hpp"
#include "Vector2D.hpp"

#include <memory>
#include <array>
#include <deque>
#include <vector>


class Effector
{
public:
	Effector();
	~Effector();

	void Update();
	void Draw();
	void Shift(const bool isSHIFT_UP, const int pow); // true : UP false : DOWN, Power Level
	static void Update_HitAnime();	// Game.cppで
	static void Draw_HitAnime();	// 任意の場所で
	bool getIsAnime();
	
	static void PlayHitAni(const double& PosX, const double& PosY);

private:
	void ShiftUp(const int powLv);
	void ShiftDown(const int powLv);
	void CopyCounter();



	std::unique_ptr<Counter> c_anime;
	std::array<Counter*, 20> c_hit;
	std::array<int, 5>	hg_shift;	// 漢字１文字ずつの画像
	std::array<int, 2>	hs_shift;	// バーン！　と　ご～ん...
	bool   f_PlayAnime;				// アニメーション再生 
	bool   f_up, f_down;			// アップかダウンか
	double x_pos, y_pos;
	int    shiftLv;					// ０～4Lv

	static const int MAX_HIT_NUM;
	static int s_c_hit[];
	static Vector2D s_hitPos[];
	static bool s_isHitAnime[];
	static int s_hg_hit[];
};

// staticメンバ変数でエフェクトを管理する？ 12/10/2015
// →static bool f_pdamage, f_edamage
// →もしくは、そのクラスごとに staticメンバ変数を用意させるかどちらが良いか。