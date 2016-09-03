#pragma once

#include "BaseScene.hpp"
#include "ISceneChanger.hpp"

#include "IScore.hpp"
#include "Graphic.hpp"
#include "Stage.hpp"
#include "StatusBoard.hpp"
#include "Vector2D.hpp"
#include "ItemMng.hpp"
#include "Bomb.hpp"


/* player */
#include "Player.hpp"
#include "Pshot.hpp"

/* enemy */
#include "BossA.hpp"
#include "EshotMng.hpp"
#include "EnemyMng.hpp"
#include "BossChara.hpp"

/* other */
#include "Effector.hpp"
#include "Graphics2D.hpp"
#include "HitEffect.hpp"

#include <memory>
#include <array>


enum eColKind
{
	eCol_CirclePoint,
	eCol_Circle,
	eCol_RectanglePoint,
	eCol_Rectangle,
};


//�Q�[����ʃN���X
class Game : public BaseScene
{
public:
	Game::Game(ISceneChanger* changer);
	~Game();
	void Initialize();
	void Update() override;     //�X�V�������I�[�o�[���C�h
	void Draw() override;       //�`�揈�����I�[�o�[���C�h
	void GetPlayerPosition(double* x, double* y){}
	void GetEnemyPosition(int index, double *x, double *y){}
	void Pause();

	static void StageCall();
	static void GameOver();
	const bool IsPause() const {
		return f_pause;
	}

private:
	void Draw_StageMsg();
	void Draw_Status();

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
	std::shared_ptr<ItemMng> itemMng;
	std::shared_ptr<HitEffect> hitEffect;
	
	bool f_pause;
};