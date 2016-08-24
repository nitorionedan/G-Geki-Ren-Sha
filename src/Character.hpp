#pragma once

#include "Task.hpp"
#include "BossChara.hpp"


enum eChara
{
	eChara_ShotA,
	eChara_ShotB,
	eChara_ShotC,
	eChara_ShotD,
	eChara_ShotE,
	eChara_ShotF,
	eChara_ShotZ,

	eChara_None,
};


class Character
{
public:
	Character(Task* task);
	~Character();

	void Update();
	void Draw();
	void ChangeShot(eChara chara);

private:
	Task* mTask;
};