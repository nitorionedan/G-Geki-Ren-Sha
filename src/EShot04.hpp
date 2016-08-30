#pragma once

#include "ShotTask.hpp"
#include "Vector2D.hpp"
#include "Effect.hpp"
#include <array>
#include <memory>


class EShot04 : public ShotTask
{
public:
	EShot04();
	~EShot04();
	virtual void Update(const double& PosX, const double& PosY, const Player& player) override;
	virtual void Draw() override;
	virtual void Fire(const double& SPEED, const double& ANGLE) override;
	virtual void Fire(const double& PosX, const double& PosY, const double& SPEED, const double& ANGLE) override;
	virtual bool IsHit(const double& ColX, const double& ColY, const double& ColR) override;

private:
	// @param[in]	id	添え字番号
	void Move(const int& id, const Player& player);

	const int ALL_FRAME_NUM;	// アニメーションの総枚数
	const int FRAME_TIME;		// １コマの表示時間（フレーム数）
	const int HIT_RANGE;		// 当たり範囲
	static const int ALL_NUM = 30;

	int gh[10];
	std::unique_ptr<Effect> effect;
	std::array<int, ALL_NUM> time;			// 経過時間
	std::array<Vector2D, ALL_NUM> pos;		// 座標
	std::array<double, ALL_NUM> vspeed;		// 弾の速さ
	std::array<double, ALL_NUM> vangle;		// 進む角度
	std::array<double, ALL_NUM> ang;		// 進む角度
	std::array<double, ALL_NUM> rad;		// 画像の回転値
	std::array<double, ALL_NUM> rota;		// 増加回転値
	std::array<double, ALL_NUM> isExist;	// 存在しているか？
};

// EOF