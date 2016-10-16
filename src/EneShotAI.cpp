#include "EneShotAI.hpp"
#include "Player.hpp"
#include <DxLib.h>


namespace 
{
	constexpr double FIRST_ANGLE = 90.;
}


void EneShotAI_Straight::Update(tShot & shot)
{
	/*shot.force.x = shot.force.Length() * std::cos(shot.force.ToRad());
	shot.force.y = shot.force.Length() * std::sin(shot.force.ToRad());
*/
	/* move */
	shot.pos += shot.force;

	/* accel */
	shot.force.x *= shot.accel;
	shot.force.y *= shot.accel;
}

// ===================================================
EneShotAI_OutsideCurve::EneShotAI_OutsideCurve()
{
	dis = 0;
	count = 0;
	variationCount = 0;
	addRad = 0;
	len = 0;
	rad = 0;
}


void EneShotAI_OutsideCurve::Update(tShot & shot)
{
	/* init */
	if (shot.time == 1)
	{
		dis = Vector2D::GetVec2(shot.pos, IPlayer::GetPos()).Length();
		variationCount = dis / shot.force.Length();
		addRad = FIRST_ANGLE / shot.force.Length();
		len = shot.force.Length();
	}

	if (shot.time % 10 == 0 && rad > -40)
	{
		rad -= 5;
		shot.force.x = shot.force.Length() * std::cos(shot.force.ToRad() + (rad * DX_PI / 180));
		shot.force.y = shot.force.Length() * std::sin(shot.force.ToRad() + (rad * DX_PI / 180));
	}

	/* calc force */
	/* move */
	shot.pos += shot.force;
	/* accel */
	shot.force.x *= shot.accel; // Do you need this var?
	shot.force.y *= shot.accel;
}

// ===================================================