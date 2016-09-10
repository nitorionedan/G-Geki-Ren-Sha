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

#include <DxLib.h>
#include <cassert>

static VECTOR const DefLightPos = VGet(100.f, 100.f, 200.f);
static int 	Screen;

bool Game::isMsg;
bool Game::isDead;


Game::Game(ISceneChanger* changer)
	: BaseScene(changer)
	, bomb(new Bomb)
	, graphic(new Graphic)
	, board(new StatusBoard)
	, c_msg(new Counter(100))
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
	/* インターフェイスクラス */
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
	stage->Finalize();

#ifdef _DEBUG
	printfDx("\n~Game");
#endif
}


void Game::Initialize()
{
	IEnemyMng::Load(IStage::GetNowStage());
	IStage::Load();

	f_pause = false;
	isMsg = false;
	isDead = false;
}


void Game::Update()
{	 
	/* ポーズ */
	if (Keyboard::Instance()->isPush(KEY_INPUT_Q))
		Pause();

	if(f_pause)
	{
		if(Keyboard::Instance()->isPush(KEY_INPUT_P))
			mSceneChanger->ChangeScene(eScene_Menu);
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

	// その他の情報
	board->Update(*player);
	itemMng->Update(player);
	
	if (isMsg)	c_msg->Update();
	if(c_msg->isLast())
	{
		c_msg->Reset();
		isMsg = false;
	}

	if (isDead)	mSceneChanger->ChangeScene(eScene_GameOver);

// TEST ----------------------------------------------
	if (DebugMode::isTest == false)	return;

	if (Keyboard::Instance()->isPush(KEY_INPUT_U))
		mSceneChanger->ChangeScene(eScene_GameOver);

	if(Keyboard::Instance()->isPush(KEY_INPUT_S))
	{
	//	PlayAnime(320, 240, eExplosion_big);
	//	bomber->PlayAnime(BossA::GetPos().x, BossA::GetPos().y);
	//	PlayAnime(100, 100, eExplosion_long);
	//	itemMng->Create(100, 100);
		IBossChara::Start();
	}
}


void Game::Draw()
{
	static int time = 0;
	time++;

	SetDrawScreen(Screen);
	ClearDrawScreen();

	// Back Ground
	stage->Draw();

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

	// ステータスボード
	board->Draw(*player);

	// 一番上に描画するその他の情報
	Draw_StageMsg();
	Draw_Status();

	score->Draw();

	SetDrawScreen(DX_SCREEN_BACK);

	//DrawRasterScroll(320, 240, 600, 100, time, Screen, TRUE); // << super noise

	int CircleX = 320;
	int CircleAngle = 0;
	// 画面を歪ませて描画
	DrawCircleScreen(
		320, 240,	// 中心座標
		80.0f + sin(CircleAngle * DX_PI_F / 180.0f) * 15.0f,	// 内側の円のサイズ
		200.0f + sin(CircleAngle * 2 * DX_PI_F / 180.0f) * 50.0f,	// 外側の円のサイズ
		48.0f,	// 内側に引き込まれるドット数
		Screen);

	// TEST-----------------------------------------------------------
	if (DebugMode::isTest == false)	return;

	VECTOR tpos = VGet(320.f, 240.f, 0.f);
	tpos = ConvScreenPosToWorldPos(tpos);
	SetLightDirection(tpos);
	DrawSphere3D(tpos, 10.f, 4, GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);
	SetLightDirection(DefLightPos);
}


void Game::Pause(){
	f_pause = !f_pause;
}


void Game::StageCall(){
	isMsg = true;
}


void Game::GameOver()
{
	isDead = true;
}


void Game::Draw_StageMsg()
{
	if (!isMsg)	return;

	const int&    X_MSG = 290, Y_MSG = 240, SPACE_MSG = 16;
	const double& EXRATE_MSG = 2.0;

	switch (IStage::GetNowStage())
	{
	case eStage::stage1:	graphic->DrawMyString2(X_MSG - 20, Y_MSG, "OPENING!", SPACE_MSG, true, EXRATE_MSG);	break;
	case eStage::stage2:	graphic->DrawMyString2(X_MSG, Y_MSG, "STAGE 2", SPACE_MSG, true, EXRATE_MSG);	break;
	}
}


void Game::Draw_Status()
{
	if (!f_pause)	return;
	graphic->DrawMyString2(290, 262, "PAUSE", 16, false, 2.0);
	graphic->DrawMyString2(20, 460, "(P) TITLE", 16, true, 2.0);
}