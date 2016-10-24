#include "EneShotAI.hpp"
#include "Player.hpp"
#include <DxLib.h>
#include <cmath>


namespace 
{
	constexpr double FIRST_ANGLE = 90.;
	const double Cos_deg10 = std::cos(10 * DX_PI / 180);
	const double Sin_deg10 = std::sin(10 * DX_PI / 180);
	const double Cos_deg20 = std::cos(20 * DX_PI / 180);
	const double Sin_deg20 = std::sin(20 * DX_PI / 180);
	const double Cos_deg30 = std::cos(30 * DX_PI / 180);
	const double Sin_deg30 = std::sin(30 * DX_PI / 180);
	const double Cos_deg40 = std::cos(40 * DX_PI / 180);
	const double Sin_deg40 = std::sin(40 * DX_PI / 180);

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
EneShotAI_Homing::EneShotAI_Homing()
{
	addSin = addCos = 0;
	time = 0;
	canHoming = true;
}


void EneShotAI_Homing::Update(tShot & shot)
{
	++time;

	if (time == 30)
	{
		addSin = Sin_deg40;
		addCos = Cos_deg40;
	}
	if (time == 40)
	{
		addSin = Sin_deg40;
		addCos = Cos_deg40;
	}
	if (time == 60)
	{
		addSin = Sin_deg20;
		addCos = Cos_deg20;
	}

	if (time % 10 == 0 && time < 150 && canHoming && time > 30)
	{
		Vector2D dir = Vector2D::GetVec2(shot.pos, IPlayer::GetPos());
		double cross = shot.force.CrossProduct(dir);
		double dot = (shot.force.x * dir.x + shot.force.y * dir.y) / (std::sqrt(std::pow(shot.force.x, 2) + std::pow(shot.force.y, 2)) * std::sqrt(std::pow(dir.x, 2) + std::pow(dir.y, 2)));
		
		if (dot > 0.9)
			canHoming = false;

		/* Rotate */
		if (cross > 0)	// left side
		{
			shot.force.x = shot.force.x * addCos - shot.force.y * addSin;
			shot.force.y = shot.force.x * addSin + shot.force.y * addCos;
		}
		else if (cross < 0) // right side
		{
			shot.force.x = shot.force.x * addCos - shot.force.y * -addSin;
			shot.force.y = shot.force.x * -addSin + shot.force.y * addCos;
		}
	}

	/* move */
	shot.pos += shot.force;
	/* accel */
	shot.force.x *= shot.accel;
	shot.force.y *= shot.accel;
} // F end
