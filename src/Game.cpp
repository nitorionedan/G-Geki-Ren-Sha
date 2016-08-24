#include "DxLib.h"

#include "Game.hpp"
#include "DebugMode.hpp"
#include "NullTask.hpp"
#include "NullBoss.hpp"


static int gh_test;

PieceEffect*	Game::pieceef;
Effect*			Game::bomber;
Effect**		Game::effect;
Score*			Game::score;
Stage*			Game::stage;
Player*			Game::player;
Pshot*			Game::pshot;
BossChara*		Game::boss;
EnemyMng*		Game::enemyMng;
ItemMng*		Game::itemMng;
Character*		Game::bossShot;
bool			Game::f_pause;
bool			Game::isMsg;
bool			Game::isDead;


Game::Game(ISceneChanger* changer)
	: BaseScene(changer)
	, graphic(new Graphic)
	, board(new Character(new StatusBoard))
	, c_msg(new Counter(100))
{
	// static-----------------------------------------------------------------
	{
		f_pause = false;
		isMsg = false;
		isDead = false;

		pieceef = new PieceEffect;

		// 200, 10, 70, 10
		bomber = new Effect(new ChargeEffect(100, 10, 70, 10));
		effect = new Effect*[EX_NUM];
		effect[0] = new Effect(new ExplosionEffect(eExplosion_small));
		effect[1] = new Effect(new ExplosionEffect(eExplosion_normal));
		effect[2] = new Effect(new ExplosionEffect(eExplosion_big));
		effect[3] = new Effect(new ExplosionEffect(eExplosion_long));

		player = new Player;
		pshot = new Pshot;
		score = new Score;
		boss = new BossChara(new NullBoss);
		enemyMng = new EnemyMng;
		stage = new Stage;
		itemMng = new ItemMng;
		bossShot = new Character(new NullTask);
	}
	// TEST
	gh_test = LoadGraph("GRAPH/GAME/Eshot/eshot03.png");

	// ~TEST
}


Game::~Game()
{
	delete player;
	delete score;
	delete stage;
	delete pshot;
	delete bossShot;
	delete boss;
	delete board;
	delete pieceef;
	delete bomber;
	{
		for (int i = 0; i < EX_NUM; i++)	delete effect[i];
		delete[] effect;
	}
	delete enemyMng;

	// TEST
	DeleteGraph(gh_test);
}


//更新
void Game::Update()
{	 
	// ポーズ---------------------------------------------	
	if (Keyboard_Get(KEY_INPUT_Q) == 1)	Pause();

	if(f_pause)
	{
		if(Keyboard_Get(KEY_INPUT_P) == 1)	mSceneChanger->ChangeScene(eScene_Menu);
		return;
	}

	// Updates--------------------------------------------
	score->Update();
	stage->Update();

	// Chara
	boss->Update();
	enemyMng->Update();
	player->Update();

	// Effect
	pieceef->Update();
	bomber->Update(BossA::GetPos().x, BossA::GetPos().y);
	for (int i = 0; i < EX_NUM; i++)	effect[i]->Update();
	
	// Shot
	pshot->Update();
	bossShot->Update();
	
	// その他の情報
	board->Update();
	itemMng->Update();
	
	if (isMsg)	c_msg->Update();
	if(c_msg->isLast())
	{
		c_msg->Reset();
		isMsg = false;
	}

	if (isDead)	mSceneChanger->ChangeScene(eScene_GameOver);

	// TEST ----------------------------------------------
	if (DebugMode::isTest == false)	return;

	if (Keyboard_Get(KEY_INPUT_B) == 1)
	{
		boss->Start(eBoss_A);
		bossShot->ChangeShot(eChara_ShotA);
	}

	if(Keyboard_Get(KEY_INPUT_R) == 1)
	{
		boss->Start(eBoss_None);
		bossShot->ChangeShot(eChara_None);
	}

	if (Keyboard_Get(KEY_INPUT_U) == 1)	mSceneChanger->ChangeScene(eScene_GameOver);

	if(Keyboard_Get(KEY_INPUT_S) == 1)
	{
	//	PlayAnime(320, 240, eExplosion_big);
	//	bomber->PlayAnime(BossA::GetPos().x, BossA::GetPos().y);
	//	PlayAnime(100, 100, eExplosion_long);
		itemMng->Create(100, 100);
	}
}


//描画
void Game::Draw()
{
	// Back Ground
	stage->Draw();

	pieceef->Draw();

	boss->Draw();
	enemyMng->Draw();

	itemMng->Draw();

	// Effect
	bomber->Draw();
	for (int i = 0; i < EX_NUM; i++)	effect[i]->Draw();

	// Character
	player->Draw();

	// Shot
	pshot->Draw();
	bossShot->Draw();

	// ステータスボード
	board->Draw();

	// 一番上に描画するその他の情報
	Draw_StageMsg();
	Draw_Status();

	score->Draw();

	// TEST-----------------------------------------------------------
	if (DebugMode::isTest == false)	return;

}


void Game::AddScore(const int & point){
	score->AddScore(point);
}


void Game::AddBomb(){
	player->AddBomb();
}


void Game::PlayAnime(const double& PlayX, const double& PlayY, eExplosion_t type)
{
	switch (type)
	{
	case eExplosion_small:
		effect[0]->PlayAnime(PlayX, PlayY);	break;
	case eExplosion_normal:
		effect[1]->PlayAnime(PlayX, PlayY);	break;
	case eExplosion_big:
		effect[1]->PlayAnime(PlayX, PlayY);
		effect[2]->PlayAnime(PlayX, PlayY);	break;
	case eExplosion_long:
		effect[3]->PlayAnime(PlayX, PlayY);	break;
	}
}


void Game::PlaySpread(const double & PosX, const double & PosY, const double & ANGLE, eSpread_t type){
	pieceef->PlayAnime(PosX, PosY, ANGLE, type);
}


void Game::PlayQuake(){
	stage->PlayQuake();
}


void Game::PlayCharge(double PosX, double PosY){
	bomber->PlayAnime(PosX, PosY);
}


void Game::LoadEnemy(eStage stage){
	enemyMng->Load(stage);
}


void Game::Pause(){
	f_pause = !f_pause;
}


void Game::ShiftReset(){
	player->ShiftReset();
}


void Game::ItemDrop(double PosX, double PosY){
	itemMng->Create(PosX, PosY);
}


void Game::ItemDrop(double PosX, double PosY, eItem_type type){
	itemMng->Create(PosX, PosY, type);
}


void Game::Shift(bool isUp){
	player->Shift(isUp);
}


void Game::StageCall(){
	isMsg = true;
}


void Game::GameOver()
{
	isDead = true;
}


void Game::DownBombNum(){
	player->DownBombNum();
}

void Game::StartBoss()
{
	boss->Start(eBoss_A);
	bossShot->ChangeShot(eChara_ShotA);
}


int Game::GetPlayerBomb(){
	return player->GetBombNum();
}


bool Game::IsPause(){
	return f_pause;
}


bool Game::IsHitPlayer(const double & myX, const double & myY)
{
	const bool& IS_HIT = player->HitCheckCircle(myX, myY);

	return IS_HIT;
}


bool Game::IsHitPlayer(const double & Range1, const double & Range2, const double & X1, const double & Y1, const double & X2, const double & Y2)
{
	const bool& IS_HIT = player->HitCheckCircle(Range1, Range2, X1, Y1, X2, Y2);

	return IS_HIT;
}


bool Game::IsHitBoss(const double& myX, const double& myY, int & dmgPoint)
{
	const bool& IS_HIT = boss->HitCheck(myX, myY, dmgPoint);
	return IS_HIT;
}


Vector2D& Game::GetPlayerPos(){
	return player->GetPos();
}


void Game::Update_Status()
{
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
// EOF