#pragma once

#include "BaseScene.hpp"
#include "ISceneChanger.hpp"

#include <memory>


/* player */
class Player;
class Pshot;
class Bomb;

/* enemy */
class EshotMng;
class EnemyMng;
class BossChara;
class EneShotCreater;

/* other */
class Score;
class ItemMng;
class StatusBoard;
class Stage;
class Effector;
class HitEffect;
class Vector2D;
class Graphic;

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
	void Pause();
	const bool IsPause() const {
		return f_pause;
	}

	static void GameOver();

private:
	void Draw_Status();

	static bool isDead;

	std::unique_ptr<Graphic> graphic;
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
	std::shared_ptr<EneShotCreater> eneShotFactory;
	
	bool f_pause;
};