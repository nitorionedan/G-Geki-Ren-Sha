#pragma once
#include "BaseScene.hpp"


class Ranking : public BaseScene
{
public:
	Ranking(ISceneChanger* changer);
	virtual void Initialize() override;		//初期化処理をオーバーライド
	virtual void Update() override;			//更新処理をオーバーライド
	virtual void Draw() override;			//描画処理をオーバーライド

private:
};