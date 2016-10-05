#include "BossAAI.hpp"
#include "Player.hpp"
#include "BossChara.hpp"
#include <functional>
#include <algorithm>
#undef min
#undef max


namespace
{
	constexpr int StartStayTime = 600;
	constexpr float StartSpeed = 2.f;
	constexpr float SC_LIMIT_XL = 78.f;
	constexpr float SC_LIMIT_XR = 560.f;
	constexpr float SC_LIMIT_YT = 74.f;
	constexpr float SC_LIMIT_YB = 406.f;

	std::function<bool (Vector2D& pos)> IsOB = [](Vector2D& pos)
	{
		const bool& IS_OUT = (pos.x < SC_LIMIT_XL ||
			SC_LIMIT_XR < pos.x ||
			pos.y < SC_LIMIT_YT ||
			SC_LIMIT_YB < pos.y);

		// 画面外に出てしまったら
		if (IS_OUT)
		{
			pos.x = std::min(std::max(pos.x, static_cast<double>(SC_LIMIT_XL)), static_cast<double>(SC_LIMIT_XR));
			pos.y = std::min(std::max(pos.y, static_cast<double>(SC_LIMIT_YT)), static_cast<double>(SC_LIMIT_YB));
		}

		return IS_OUT;
	};
}


BossAAI_start::BossAAI_start()
{
	pos.SetVec(400.f, -100.f);
	c_start = 0;
}


void BossAAI_start::Update()
{
	if (c_start < StartStayTime)
		++c_start;

	if (c_start != StartStayTime)
		return;

	pos.x -= StartSpeed;
	pos.y += 1.5f;
}


void BossAAI_normal::Update()
{
	//const float& tarX = IPlayer::GetPos().x;
	//const float& tarY = IPlayer::GetPos().y;

	//// 自機の方向を計算
	//if (IsOB(pos))
	//	angle = std::atan2(tarY - pos.y, tarX - pos.x);

	//// 弾に当たったときスピードを落とす
	//if ()
	//{
	//	pos.x += std::cos(angle) * SPEED * HitBrake;
	//	pos.y += std::sin(angle) * SPEED * HitBrake;
	//}
	//else {
	//	pos.x += std::cos(angle) * SPEED;
	//	pos.y += std::sin(angle) * SPEED;
	//}

	//if (c_atk1->isLast())	Eshot::GoFire1();

	//c_atk1->Update();
}


void BossAAI_weak::Update()
{
}


void BossAAI_dead::Update()
{
}


void BossAAI_end::Update()
{
}
