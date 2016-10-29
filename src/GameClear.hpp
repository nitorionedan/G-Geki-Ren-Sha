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

	// �t�F�[�h�C��
	int time_in;

	// �t�F�[�h�A�E�g
	int time_out;
};