#pragma once
#include "BaseScene.hpp"


class Ranking : public BaseScene
{
public:
	Ranking(ISceneChanger* changer);
	virtual void Initialize() override;		//�������������I�[�o�[���C�h
	virtual void Update() override;			//�X�V�������I�[�o�[���C�h
	virtual void Draw() override;			//�`�揈�����I�[�o�[���C�h

private:
};