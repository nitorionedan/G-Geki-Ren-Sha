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
#include <fstream>
#include <DxLib.h>
#include <cassert>

static VECTOR const DefLightPos = VGet(100.f, 100.f, 200.f);
static int 	Screen;
static int gh_flyer;

bool Game::isDead;

namespace
{
	Vector2D pos(320, 240);
}


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
	, eneShot(new EneShot)
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
	gh_flyer = LoadGraph("GRAPH/GAME/ENEMY/flyer_test.png");

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
	IEneShot::reset();

	DeleteGraph(gh_flyer);
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
	eneShot->Update();

	// Bomber
	bomb->Update();

	// その他の情報
	board->Update(*player);
	itemMng->Update(player);
	
	if (isDead)	mSceneChanger->ChangeScene(eScene::gameOver);

	static int c_test = 0;
	if (c_test != 0)
		++c_test;
	if (Keyboard::Instance()->isPush(KEY_INPUT_C))
	{
		c_test = 1;

		//for (int i = 0; i < 3; ++i)
		//{
		//	Vector2D dir = Vector2D::GetVec((GetRand(20) - 10) / 10., (GetRand(20) - 10) / 10.);
		//	Vector2D force = dir.Normalize() * 7;
		//	IEneShot::Fire_Ang(eShotType::missile, pos, 0, 4, dir.ToRad(), 1.01, 2, eShotAI::homing);
		//}
		//IStage::PlayDist(GetRand(640), GetRand(480));
	}

	if (c_test == 5 || c_test == 10 || c_test == 15)
	{
		Vector2D dir = Vector2D::GetVec2(pos, Vector2D::ZERO);
		Vector2D force = dir.Normalize() * 3;
		IEneShot::Fire(eShotType::laser, pos, 0, force, 1, 0);
		if (c_test == 15)
			c_test = 0;
	}

// TEST ----------------------------------------------
	if (DebugMode::isTest == false)	return;

	if (Keyboard::Instance()->isDown(KEY_INPUT_Q) && Keyboard::Instance()->isDown(KEY_INPUT_W))
		mSceneChanger->ChangeScene(eScene::gameOver);

	if(Keyboard::Instance()->isPush(KEY_INPUT_F2))
		IBossChara::Start();

	if (Keyboard::Instance()->isPush(Input::KeyCode.C))
		effector->PlayAnime(320, 240, eExplosion_small);
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
	effector->Draw_Smoke();
	effector->Draw_Explosion();
	itemMng->Draw();

	// Character
	player->Draw();

	// Bomb
	bomb->Draw();

	// bullet
	eneShot->Draw();

	hitEffect->Draw();

	// Shot
	pshot->Draw();

	// ステータスボード
	board->Draw(*player);

	// 一番上に描画するその他の情報
	Draw_Status();

	score->Draw();

	//static float count = 0;
	//count += 0.01f;
	//DrawRotaGraph(320 + std::cos(count) * 30, 170 + std::sin(count) * 30, 2, 0, gh_flyer, true);

	//DrawRasterScroll(320, 240, 600, 100, time, Screen, TRUE); // << super noise

	//int CircleX = 320;
	//int CircleAngle = 0;
	//// 画面を歪ませて描画
	//DrawCircleScreen(
	//	320, 240,	// 中心座標
	//	//80.0f + sin(CircleAngle * DX_PI_F / 180.0f) * 15.0f,	// 内側の円のサイズ
	//	tmp,
	//	//200.0f + sin(CircleAngle * 2 * DX_PI_F / 180.0f) * 50.0f,	// 外側の円のサイズ
	//	0.f,
	//	0.f,	// 内側に引き込まれるドット数
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