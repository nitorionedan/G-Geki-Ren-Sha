#pragma once

#include "BaseScene.hpp"
#include "ISceneChanger.hpp"

#include "Score.hpp"
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


//�Q�[����ʃN���X
class Game : public BaseScene
{
public:
	Game::Game(ISceneChanger* changer);
	~Game();
	void Update() override;     //�X�V�������I�[�o�[���C�h
	void Draw() override;       //�`�揈�����I�[�o�[���C�h
	void GetPlayerPosition(double* x, double* y){}
	void GetEnemyPosition(int index, double *x, double *y){}

	static void AddScore(const int& point);
	static void PlayQuake();
	static void LoadEnemy(eStage stage);
	static void Pause();
	static void ItemDrop(double PosX, double PosY);
	static void ItemDrop(double PosX, double PosY, eItem_type type);
	static void StageCall();
	static void GameOver();
	static void StartBoss();
	static bool IsPause();

	// @brief	���@�̉~�Ƒ��̓_�̓����蔻��
	bool IsHitPlayer(const double& myX, const double& myY);

	// @brief	���@�̉~�Ƒ��̉~�̓����蔻��
	bool IsHitPlayer(const double& Range1, const double& Range2,
		const double & X1, const double& Y1, const double& X2, const double& Y2);

	static bool IsHitBoss(const double& myX, const double& myY, int& dmgPoint);

private:
	void Update_Status();
	void Draw_StageMsg();
	void Draw_Status();

	static Score* score;
	static Stage* stage;
	static BossChara* boss;
	static EnemyMng* enemyMng;
	static ItemMng*	itemMng;
	static bool f_pause;
	static bool isMsg;
	static bool isDead;

	std::unique_ptr<Graphic> graphic;
	std::unique_ptr<Counter> c_msg;
	std::shared_ptr<Player> player;
	std::shared_ptr<StatusBoard> board;
	std::shared_ptr<Pshot> pshot;
	std::shared_ptr<Effector> effector;
	std::shared_ptr<Bomb> bomb;

	// TEST
};