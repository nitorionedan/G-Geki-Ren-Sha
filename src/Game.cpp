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

/* other */
#include "Score.hpp"
#include "ItemMng.hpp"
#include "StatusBoard.hpp"
#include "Stage.hpp"
#include "Effector.hpp"
#include "ExplosionEffect.hpp"
#include "Graphics2D.hpp"
#include "Graphic.hpp"
#include "HitEffect.hpp"
#include "Vector2D.hpp"
#include "Keyboard.hpp"
#include "Sound.hpp"
#include "EneShot.hpp"
#include "FileDef.h"
#include <fstream>
#include <DxLib.h>
#include <cassert>

static VECTOR const DefLightPos = VGet(100.f, 100.f, 200.f);
static int 	Screen;
static int gh_test;

bool Game::isDead;

namespace
{
	Vector2D pos(320, 240);
}


Game::Game(ISceneChanger* changer)
	: BaseScene(changer)
	, bomb(     new Bomb)
	, graphic(  new Graphic)
	, board(    new StatusBoard)
	, player(   new Player)
	, pshot(    new Pshot)
	, effector( new Effector)
	, score(    new Score)
	, boss(     new BossChara(new NullBoss))
	, enemyMng( new EnemyMng)
	, stage(    new Stage)
	, itemMng(  new ItemMng)
	, hitEffect(new HitEffect)
	, eneShot(  new EneShot)
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
	IEneShot::set(eneShot);

	Screen = MakeScreen(640, 480, TRUE);
	gh_test = LoadGraph(MyFile::Gr::EFCT_THUNDER);

	Initialize();
}


Game::~Game()
{
	/* free interface */
	IScore::reset();
	IEnemyMng::reset();
	IStage::reset();
	IPlayer::reset();
	IBossChara::reset();
	IItemMng::reset();
	IBomb::reset();
	IHitEffect::reset();
	IPshot::reset();
	IEneShot::reset();

	DeleteGraph(gh_test);
}


void Game::Initialize()
{
	IEnemyMng::Load(IStage::GetNowStage());
	IStage::Load();

	time = 0;
	f_pause = false;
	isDead = false;
}


void Game::Update()
{	 
	++time;

	/* pause */
	if (Keyboard::Instance()->isPush(KEY_INPUT_Q))
		Pause();

	/* force gaover */
	if (CheckHitKey(KEY_INPUT_Q) != 0 && CheckHitKey(KEY_INPUT_W) != 0)
		Game::GameOver();

	if(f_pause)
	{
		if (Keyboard::Instance()->isPush(KEY_INPUT_P))
			mSceneChanger->ChangeScene(eScene::menu);
		return;
	}

	/* Updates */
	score->Update();
	stage->Update();
	boss->Update();
	enemyMng->Update(*player);
	player->Update();
	effector->Update();
	hitEffect->Update();
	pshot->Update();        // players shot
	eneShot->Update();
	bomb->Update();
	board->Update(*player); // status board
	itemMng->Update(player);
	
	if (isDead)
		mSceneChanger->ChangeScene(eScene::gameOver);

	if (IStage::CheckAllClear())
		mSceneChanger->ChangeScene(eScene::gameClear);

// TEST ----------------------------------------------
	if (DebugMode::isTest)
	{

		if (Keyboard::Instance()->isPush(Input::KeyCode.X)) {
			//effector->PlayCyclone(player->GetPos().x, player->GetPos().y);
			effector->PlayCyclone(320, 240);
		}

		if (Keyboard::Instance()->isDown(KEY_INPUT_Q) && Keyboard::Instance()->isDown(KEY_INPUT_W))
			mSceneChanger->ChangeScene(eScene::gameOver);

		if (Keyboard::Instance()->isPush(KEY_INPUT_F2))
			IBossChara::Start();
	}
}


void Game::Draw()
{
	/*static float c_tmp = 0.f;
	c_tmp += 0.01f;
	float tmp = std::sin(c_tmp) * 300;*/

	stage->Draw();
	effector->Draw_Shock();
	boss->Draw();
	effector->Draw();
	enemyMng->Draw();
	effector->Draw_Smoke();
	effector->Draw_Explosion();    // explosions
	effector->Draw_SpecialExplo(); // boss only explosion
	itemMng->Draw();
	player->Draw();
	bomb->Draw();
	eneShot->Draw();
	hitEffect->Draw();
	effector->Draw_Cyclone();
	//effector->Draw_Test();
	pshot->Draw();	               // players shot
	board->Draw(*player);          // status board
	Draw_Status();                 // other infos
	score->Draw();

	// TEST-----------------------------------------------------------
	if (DebugMode::isTest)
	{
		// Thunder test
		/*if (CheckHitKey(KEY_INPUT_C) != 0 && time % 8 >= 2)
		{
			double rnd = GetRand(360) * DX_PI / 180.;
			int irnd = GetRand(2);
			DrawRotaGraph(320, 240, 2, 0, gh_test, TRUE, irnd);
		}*/
	}
}


void Game::Pause() {
	f_pause = !f_pause;
}


void Game::GameOver()
{
	isDead = true;

	/* create continue data */
	std::ofstream ofs("./data/continue.dat");
	int tmp = static_cast<int>(IStage::GetNowStage());
	ofs << tmp;

	ofs.close();
}


void Game::Draw_Status()
{
	if (!f_pause)	return;
	graphic->DrawMyString2(290, 262, "PAUSE", 16, false, 2.0);
	graphic->DrawMyString2(20, 460, "(P) TITLE", 16, true, 2.0);
}