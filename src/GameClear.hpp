#pragma once
#include "BaseScene.hpp"
#include "ISceneChanger.hpp"


class GameClear : public BaseScene
{
public:
	explicit GameClear(ISceneChanger* changer);
	~GameClear();
	virtual void Update() override;
	virtual void Draw() override;

private:
	int gh;
	int time;

	// フェードイン
	int time_in;

	// フェードアウト
	int time_out;
};