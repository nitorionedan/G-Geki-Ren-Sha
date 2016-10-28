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
		constexpr const char* SMOKE00          = "GRAPH/GAME/EFFECT/smoke00.png";
		constexpr const char* SMOKE01          = "GRAPH/GAME/EFFECT/smoke01.png";
		constexpr const char* SHOCK            = "GRAPH/GAME/EFFECT/circle.png";
		constexpr const char* ENEMY_ZE         = "GRAPH/GAME/ENEMY/enemy00s.png"; // 29 x 19
		constexpr const char* ENEMY_DE00       = "GRAPH/GAME/ENEMY/ene02.png";
		constexpr const char* ENEMY_CAREER     = "GRAPH/GAME/ENEMY/career.png";

		constexpr const char* ENEMY_FLWR_CORE  = "GRAPH/GAME/ENEMY/core.png";
		constexpr const char* ENEMY_FLWR_PETAL = "GRAPH/GAME/ENEMY/petal.png";
		
		constexpr const char* FIRE             = "GRAPH/GAME/Eshot/efire0.png";
		constexpr const char* ESHOT_LASER      = "GRAPH/GAME/Eshot/laser.png";
		constexpr const char* ESHOT_MISSILE    = "GRAPH/GAME/Eshot/missile.png";
		constexpr const char* ESHOT_SHORT      = "GRAPH/GAME/Eshot/eshot01.png";
		constexpr const char* PLANE_BLACK      = "GRAPH/GAME/BACKGROUND/blackbg.png";
	}

	/* sound files */
	namespace Se
	{
		constexpr const char* SMALL_EX   = "SOUND/SE/explosion06.wav";
		constexpr const char* NORML_EX   = "SOUND/SE/explosion04.mp3";
		constexpr const char* LARGE_EX   = "SOUND/SE/explosion03.mp3";
		constexpr const char* LONG_EX    = "SOUND/SE/explosion04.mp3";
		constexpr const char* STAGE1     = "";
		constexpr const char* SHOT_LASER = "SOUND/SE/eshot00.wav";
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