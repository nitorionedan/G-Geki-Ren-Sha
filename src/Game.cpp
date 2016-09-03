#include <DxLib.h>
#include "Game.hpp"
#include "DebugMode.hpp"
#include "NullEnemyShot.hpp"
#include "NullBoss.hpp"


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
{
	// these are must funcs
	player->setup(bomb, pshot, stage);
	enemyMng->setup(boss);

	/* 自分のクラスで吸えばいいじゃん・・・ */
	IScore::set(score);
	IEnemyMng::set(enemyMng);
	IStage::set(stage);
	IPlayer::set(player);
	IBossChara::set(boss);
	IItemMng::set(itemMng);
	IBomb::set(bomb);
	IHitEffect::set(hitEffect);

	Initialize();
}


Game::~Game()
{
}


void Game::Initialize()
{
	IEnemyMng::Load(eStage::stage1);
	IStage::Load(eStage::stage1);

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

	if (Keyboard::Instance()->isPush(KEY_INPUT_U))	mSceneChanger->ChangeScene(eScene_GameOver);

	if(Keyboard::Instance()->isPush(KEY_INPUT_S))
	{
	//	PlayAnime(320, 240, eExplosion_big);
	//	bomber->PlayAnime(BossA::GetPos().x, BossA::GetPos().y);
	//	PlayAnime(100, 100, eExplosion_long);
		itemMng->Create(100, 100);
	}
}


void Game::Draw()
{
	// Back Ground
	stage->Draw();

	boss->Draw();

	// Effect
	effector->Draw();

	enemyMng->Draw();


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

	// TEST-----------------------------------------------------------
	if (DebugMode::isTest == false)	return;

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

	switch (stage->nowStage)
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