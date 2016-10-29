#pragma once


enum class eScene
{
	menu,	  // ���j���[���
	game,	  // �Q�[�����
	gameOver, // �K���I�x��
	config,	  // �ݒ���
	another,  // �閧
	ranking,
	gameClear,

	none,	  // ����
};


//�V�[����ύX���邽�߂̃C���^�[�t�F�C�X�N���X
class ISceneChanger
{
public:
	virtual ~ISceneChanger() = 0;
	virtual void ChangeScene(eScene NextScene) = 0; //�w��V�[���ɕύX����
};