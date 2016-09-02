#pragma once

#include "BaseScene.hpp"
#include "ISceneChanger.hpp"

#include "IScore.hpp"
#include "Graphic.hpp"
#include "Stage.hpp"
#include "StatusBoard.hpp"
#include "Vector2D.hpp"
#include "ItemMng.hpp"

/* player */
#include "Player.hpp"
#include "Pshot.hpp"
#include "Bomb.hpp"

/* enemy */
#include "BossA.hpp"
#include "EshotMng.hpp"
#include "EnemyMng.hpp"
#include "BossChara.hpp"

/* other */
#include "Effector.hpp"
#include "Graphics2D.hpp"

#include <memory>
#include <array>


enum eColKind
{
	eCol_CirclePoint,
	eCol_Circle,
	eCol_RectanglePoint,
	eCol_Rectangle,
};


//ゲーム画面クラス
class Game : public BaseScene
{
public:
	Game::Game(ISceneChanger* changer);
	~Game();
	void Initialize();
	void Update() override;     //更新処理をオーバーライド
	void Draw() override;       //描画処理をオーバーライド
	void GetPlayerPosition(double* x, double* y){}
	void GetEnemyPosition(int index, double *x, double *y){}
	void Pause();

	static void ItemDrop(double PosX, double PosY);
	static void ItemDrop(double PosX, double PosY, eItem_type type);
	static void StageCall();
	static void GameOver();
	const bool IsPause() const {
		return f_pause;
	}

	// @brief	自機の円と他の点の当たり判定
	bool IsHitPlayer(const double& myX, const double& myY);

	// @brief	自機の円と他の円の当たり判定
	bool IsHitPlayer(const double& Range1, const double& Range2,
		const double & X1, const double& Y1, const double& X2, const double& Y2);

	static bool IsHitBoss(const double& myX, const double& myY, int& dmgPoint);

private:
	void Draw_StageMsg();
	void Draw_Status();

	static ItemMng*	itemMng;
	static bool isMsg;
	static bool isDead;

	std::unique_ptr<Graphic> graphic;
	std::unique_ptr<Counter> c_msg;
	std::shared_ptr<Player> player;
	std::shared_ptr<StatusBoard> board;
	std::shared_ptr<Pshot> pshot;
	std::shared_ptr<Effector> effector;
	std::shared_ptr<Bomb> bomb;
	std::shared_ptr<Score> score;
	std::shared_ptr<Stage> stage;
	std::shared_ptr<BossChara> boss;
	std::shared_ptr<EnemyMng> enemyMng;
	
	bool f_pause;
};