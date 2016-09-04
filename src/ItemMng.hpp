#pragma once

#include "Vector2D.hpp"

#include <array>
#include <memory>


class Player;


enum eItem
{
	Bomber,
	Power,
	Shield,
};


class ItemMng
{
public:
	ItemMng();
	~ItemMng();
	void Update(std::shared_ptr<Player> player);
	void Draw();

	// @brief	アイテムランダムドロップ
	void Create(double PosX, double PosY);

	// @brief	アイテム指定ドロップ
	void Create(double PosX, double PosY, eItem type);

	static const int NUM = 10;

private:
	void Move(const int& id);
	void HitCheck(const int& id, std::shared_ptr<Player> player);
	void Reset(double PosX, double PosY, int id);

	const double LIMIT_L;
	const double LIMIT_R;
	const double LIMIT_T;
	const double LIMIT_B;
	const int DELETE_TIME;
	const int HIT_RANGE;

	int gh_p[32],
		gh_b[32],
		gh_s[32];
	int sh_get;

	std::array<eItem, NUM> type;
	std::array<Vector2D, NUM> pos;
	std::array<Vector2D, NUM> dir;
	std::array<Vector2D, NUM> vel;
	std::array<int, NUM> time;
	std::array<double, NUM> vang;
	std::array<bool, NUM> isExist;
	std::array<bool, NUM> isDeleting;
};


class IItemMng 
{
public:
	~IItemMng(){}
	static void set(std::shared_ptr<ItemMng> itemMng);
	static void Create(Vector2D pos) {
		mItemMng->Create(pos.x, pos.y);
	}
	static void Create(Vector2D pos, eItem type) {
		mItemMng->Create(pos.x, pos.y, type);
	}

private:
	IItemMng() { mItemMng = nullptr; }

	static std::shared_ptr<ItemMng> mItemMng;
};