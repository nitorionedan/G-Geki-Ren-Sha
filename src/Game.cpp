#include "Game.hpp"
#include "DebugMode.hpp"
#include "IScore.hpp"

/* player */
#include "Player.hpp"
#include "Pshot.hpp"
#include "Bomb.hpp"

/* enemy */
#include "EshotMng.hpp"
#include "NullEnemyShot.hpp"
#include "EnemyMng.hpp"
#include "BossChara.hpp"
#include "NullBoss.hpp"
#include "EneShotCreater.hpp"

/* other */
#include "Score.hpp"
#include "ItemMng.hpp"
#include "StatusBoard.hpp"
#include "Stage.hpp"
#include "Effector.hpp"
#include "Graphics2D.hpp"
#include "Graphic.hpp"
#include "HitEffect.hpp"
#include "Vector2D.hpp"
#include "Keyboard.hpp"
#include "Sound.hpp"

#include <DxLib.h>
#include <cassert>

static VECTOR const DefLightPos = VGet(100.f, 100.f, 200.f);
static int 	Screen;

bool Game::isDead;


Game::Game(ISceneChanger* changer)
	: BaseScene(changer)
	, bomb(new Bomb)
	, graphic(new Graphic)
	, board(new StatusBoard)
	, player(new Player)
	, pshot(new Pshot)
	, effector(new Effector)
	, score(new Score)
	, boss(new BossChara(new NullBoss))
	, enemyMng(new EnemyMng)
	, stage(new Stage)
	, itemMng(new ItemMng)
	, hitEffect(new HitEffect)
	, eneShotFactory(new EneShotCreater)
{
	/* interface-class */
	IScore::set(score);
	IEnemyMng::set(enemyMng);
	IStage::set(stage);
	IPlayer::set(player);
	IBossChara::set(boss);
	IItemMng::set(itemMng);
	IBomb::set(bomb);
	IHitEffect::set(hitEffect);
	IPshot::set(pshot);
	IEneShotCreater::set(eneShotFactory);

	Screen = MakeScreen(640, 480, TRUE);

	Initialize();
}


Game::~Game()
{
	IScore::reset();
	IEnemyMng::reset();
	IStage::reset();
	IPlayer::reset();
	IBossChara::reset();
	IItemMng::reset();
	IBomb::reset();
	IHitEffect::reset();
	IPshot::reset();
	IEneShotCreater::reset();
}


void Game::Initialize()
{
	IEnemyMng::Load(IStage::GetNowStage());
	IStage::Load();

	f_pause = false;
	isDead = false;
}


void Game::Update()
{	 
	/* �|�[�Y */
	if (Keyboard::Instance()->isPush(KEY_INPUT_Q))
		Pause();

	if(f_pause)
	{
		if (Keyboard::Instance()->isPush(KEY_INPUT_P))
		{
			mSceneChanger->ChangeScene(eScene_Menu);
			Sound::Stop();
		}
		return;
	}

	score->Update();
	stage->Update();

	// Chara
	boss->Update();
	enemyMng->Update(*player);
	player->Update();

	// Effect
	effector->Update();
	hitEffect->Update();
	
	// Shot
	pshot->Update();
	eneShotFactory->Update();

	// Bomber
	bomb->Update();

	// ���̑��̏��
	board->Update(*player);
	itemMng->Update(player);
	
	if (isDead)	mSceneChanger->ChangeScene(eScene_GameOver);

// TEST ----------------------------------------------
	if (DebugMode::isTest == false)	return;

	if (Keyboard::Instance()->isDown(KEY_INPUT_Q) && Keyboard::Instance()->isDown(KEY_INPUT_W))
		mSceneChanger->ChangeScene(eScene_GameOver);

	if(Keyboard::Instance()->isPush(KEY_INPUT_F2))
	{
		IBossChara::Start();
		//IStage::SkipTo(1000);
	}
}


void Game::Draw()
{
	static int time = 0;
	time++;

	/*static float c_tmp = 0.f;
	c_tmp += 0.01f;
	float tmp = std::sin(c_tmp) * 300;*/

	// Back Ground
	stage->Draw();

	effector->Draw_Shock();

	boss->Draw();

	// Effect
	effector->Draw();

	enemyMng->Draw();
	eneShotFactory->Draw();

	itemMng->Draw();

	// Character
	player->Draw();

	hitEffect->Draw();

	// Shot
	pshot->Draw();

	// Bomb
	bomb->Draw();

	// �X�e�[�^�X�{�[�h
	board->Draw(*player);

	// ��ԏ�ɕ`�悷�邻�̑��̏��
	Draw_Status();

	score->Draw();

	//DrawRasterScroll(320, 240, 600, 100, time, Screen, TRUE); // << super noise

	//int CircleX = 320;
	//int CircleAngle = 0;
	//// ��ʂ�c�܂��ĕ`��
	//DrawCircleScreen(
	//	320, 240,	// ���S���W
	//	//80.0f + sin(CircleAngle * DX_PI_F / 180.0f) * 15.0f,	// �����̉~�̃T�C�Y
	//	tmp,
	//	//200.0f + sin(CircleAngle * 2 * DX_PI_F / 180.0f) * 50.0f,	// �O���̉~�̃T�C�Y
	//	0.f,
	//	0.f,	// �����Ɉ������܂��h�b�g��
	//	Screen);

	// TEST-----------------------------------------------------------
	if (DebugMode::isTest == false)	return;

	/*VECTOR tpos = VGet(320.f, 240.f, 0.f);
	tpos = ConvScreenPosToWorldPos(tpos);
	SetLightDirection(tpos);
	DrawSphere3D(tpos, 10.f, 4, GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);
	SetLightDirection(DefLightPos);*/
}


void Game::Pause(){
	f_pause = !f_pause;
}


void Game::GameOver()
{
	isDead = true;
}


void Game::Draw_Status()
{
	if (!f_pause)	return;
	graphic->DrawMyString2(290, 262, "PAUSE", 16, false, 2.0);
	graphic->DrawMyString2(20, 460, "(P) TITLE", 16, true, 2.0);
}