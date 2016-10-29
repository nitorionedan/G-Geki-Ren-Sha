#pragma once
/*
@brief	file path
@author ñÓïóÅI
*/

namespace MyFile
{
	/* image files */
	namespace Gr//beruberu18
	{
		constexpr const char* GAME_CLEAR = "GRAPH/GAME/GameClear.png";

		constexpr const char* SMOKE00          = "GRAPH/GAME/EFFECT/smoke00.png";
		constexpr const char* SMOKE01          = "GRAPH/GAME/EFFECT/smoke01.png";
		constexpr const char* SHOCK            = "GRAPH/GAME/EFFECT/circle.png";

		constexpr const char* ENEMY_ZE         = "GRAPH/GAME/ENEMY/enemy00s.png"; // 29 x 19
		constexpr const char* ENEMY_DE00       = "GRAPH/GAME/ENEMY/ene02.png";
		constexpr const char* ENEMY_CAREER     = "GRAPH/GAME/ENEMY/career.png";
		constexpr const char* ENEMY_FLWR_CORE  = "GRAPH/GAME/ENEMY/core.png";
		constexpr const char* ENEMY_FLWR_PETAL = "GRAPH/GAME/ENEMY/petal.png";
		
		constexpr const char* BOSS_FLYER_BACK      = "GRAPH/GAME/ENEMY/flyer_back.png";
		constexpr const char* BOSS_FLYER_HEAD      = "GRAPH/GAME/ENEMY/flyer_head.png";
		constexpr const char* BOSS_FLYER_WING_HEAD = "GRAPH/GAME/ENEMY/flyer_headWing.png";
		constexpr const char* BOSS_FLYER_WING_BACK = "GRAPH/GAME/ENEMY/flyer_wingBack.png";
		constexpr const char* BOSS_FLYER_TAIL00    = "GRAPH/GAME/ENEMY/flyer_tail00.png";
		constexpr const char* BOSS_FLYER_TAIL01    = "GRAPH/GAME/ENEMY/flyer_tail01.png";
		constexpr const char* BOSS_FLYER_GUN       = "GRAPH/GAME/ENEMY/flyer_gun.png";
		constexpr const char* BOSS_FLYER_L_WING    = "GRAPH/GAME/ENEMY/flyer_Lwing.png";
		
		constexpr const char* FIRE             = "GRAPH/GAME/Eshot/efire0.png";
		constexpr const char* ESHOT_LASER      = "GRAPH/GAME/Eshot/laser.png";
		constexpr const char* ESHOT_MISSILE    = "GRAPH/GAME/Eshot/missile.png";
		constexpr const char* ESHOT_SHORT      = "GRAPH/GAME/Eshot/eshot01.png";

		constexpr const char* PLANE_BLACK      = "GRAPH/GAME/BACKGROUND/blackbg.png";
		
		constexpr const char* EFCT_SPECIAL     = "GRAPH/GAME/EFFECT/clear_explo.png";
	}

	/* sound files */
	namespace Se
	{
		constexpr const char* SMALL_EX     = "SOUND/SE/explosion06.wav";
		constexpr const char* NORML_EX     = "SOUND/SE/explosion04.mp3";
		constexpr const char* LARGE_EX     = "SOUND/SE/explosion03.mp3";
		constexpr const char* LONG_EX      = "SOUND/SE/explosion04.mp3";
		constexpr const char* STAGE1       = "";
		constexpr const char* SHOT_LASER   = "SOUND/SE/eshot00.wav";
		constexpr const char* SHOT_MISSILE = "SOUND/SE/missile00.mp3";
	}

	/* data files */
	namespace Dat
	{
		constexpr const char* ENEMY_OPENING = "data/EnemyTableOP.csv";
		constexpr const char* ENEMY_STG1    = "data/EnemyTable1.csv";
		constexpr const char* CONTINUE      = "./data/continue.dat";
		constexpr const char* RANKING       = "./data/rkg.dat";
	}
}