#pragma once


#include "BaseScene.hpp"
#include "ISceneChanger.hpp"


class GameOver : public BaseScene
{
public:
	GameOver::GameOver(ISceneChanger* changer);
	~GameOver();
	virtual void Update() override;     //更新処理をオーバーライド
	virtual void Draw() override;       //描画処理をオーバーライド

private:
	GameOver();

	int gh;
	int time;

	// フェードイン
	int time_in;

	// フェードアウト
	int time_out;
};