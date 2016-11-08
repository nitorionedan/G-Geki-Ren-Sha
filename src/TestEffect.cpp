#include "TestEffect.hpp"
#include <DxLib.h>


TestEffect::TestEffect()
	: maxRadius(40)
	, minRadius(33)
{
	m_radius = minRadius;
}


TestEffect::~TestEffect()
{
}


void TestEffect::Update()
{
	if (m_radius > minRadius)
	{
		m_radius -= 0.7;
		if (m_radius < minRadius)
			m_radius = minRadius;
	}
}


void TestEffect::Draw()
{
	DrawCircle(520, 440, m_radius, GetColor(255, 0, 0), TRUE);
}


void TestEffect::PlayAnime()
{
	m_radius = maxRadius;
}
