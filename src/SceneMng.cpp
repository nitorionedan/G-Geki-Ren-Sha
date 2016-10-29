#include <DxLib.h>
#include <cassert>

/* scenes */
#include "Config.hpp"
#include "Game.hpp"
#include "GameOver.hpp"
#include "Menu.hpp"
#include "Ranking.hpp"
#include "GameClear.hpp"

#include "SceneMng.hpp"
#include "Another.hpp"
#include "Keyboard.hpp"
#include "DebugMode.hpp"
#include "Sound.hpp"


SceneMng::SceneMng() :	mNextScene(eScene::none){
	mScene = static_cast<BaseScene*> (new Menu(this));	// �ŏ��̃V�[��
}


SceneMng::~SceneMng() {
	delete mScene;
}


//�X�V
void SceneMng::Update()
{			
	DebugMode::Update();

	if (mNextScene != eScene::none)								// ���̃V�[�����Z�b�g����Ă�����
	{    
		delete mScene;											// ���݂̃V�[���̏I�����������s
		switch (mNextScene)										// �V�[���ɂ���ď����𕪊�
		{
		case eScene::menu:										// ���̉�ʂ����j���[�Ȃ�
			mScene = static_cast<BaseScene*> (new Menu(this));	// ���j���[��ʂ̃C���X�^���X�𐶐�����
			break;												// �ȉ���
		case eScene::game:     mScene = static_cast<BaseScene*>(new Game(this)); break;
		case eScene::config:   mScene = static_cast<BaseScene*>(new Config(this)); break;
		case eScene::another:  mScene = static_cast<BaseScene*>(new Another(this)); break;
		case eScene::gameOver: mScene = static_cast<BaseScene*>(new GameOver(this)); break;
		case eScene::ranking:  mScene = static_cast<BaseScene*>(new Ranking(this)); break;
		case eScene::gameClear:  mScene = static_cast<BaseScene*>(new GameClear(this)); break;
		default:	assert(!"SceneMng::Update()");
		}
		mNextScene = eScene::none;								// ���̃V�[�������N���A
	}
	mScene->Update();											// �V�[���̍X�V

	// DEBUG ONLY
	if (Keyboard::Instance()->isPush(KEY_INPUT_F1))
		DebugMode::SwitchTest();
}


//�`��
void SceneMng::Draw()
{
	DebugMode::Draw();
	mScene->Draw();
}


// ���� nextScene �ɃV�[����ύX����
void SceneMng::ChangeScene(eScene NextScene)
{
	mNextScene = NextScene; // set next scene
	Sound::Stop();
}