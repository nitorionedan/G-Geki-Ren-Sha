#include "SpecialExplosionEffect.hpp"
#include "FileDef.h"
#include <DxLib.h>
#include <algorithm>

namespace
{
	constexpr double ADD_RANGE = 0.3;
	constexpr int PLAY_TIME = 60;
}


SpecialExplosionEffect::SpecialExplosionEffect()
{
	gh = LoadGraph(MyFile::Gr::EFCT_SPECIAL);

	m_effect.reserve(10);
}


SpecialExplosionEffect::~SpecialExplosionEffect()
{
	DeleteGraph(gh);
}


void SpecialExplosionEffect::Draw()
{
	for (auto i : m_effect)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, i.alpha);
		DrawRotaGraph(i.pos.x, i.pos.y, i.range, 0., gh, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}


void SpecialExplosionEffect::Update()
{
	/* time count */
	for (auto & i : m_effect)
		++i.time;

	/* erase obj */
	m_effect.erase(std::remove_if(std::begin(m_effect), std::end(m_effect),
		[](const tEffect& effect)->bool
	{
		return (effect.time >= PLAY_TIME);
	}), std::end(m_effect));

	/* increase a range */
	for (auto& i : m_effect)
		i.addRange += ADD_RANGE;
	for (auto& i : m_effect)
		i.range += i.addRange;

	/* increase a alpha */
	for (auto& i : m_effect)
		i.alpha -= i.addAlpha;
}


void SpecialExplosionEffect::Play(double x, double y)
{
	tEffect tmp;
	tmp.pos.SetVec(x, y);
	tmp.addRange = 0.1;
	tmp.range    = 1.;
	tmp.alpha  = 255.;
	tmp.addAlpha = 255 / PLAY_TIME;
	tmp.time     = 0;
	tmp.isPlay   = true;

	m_effect.emplace_back(tmp);
}


bool tEffect::operator==(const tEffect & other) const
{
	return (pos == other.pos) &&
		(range == other.range) &&
		(addRange == other.addRange) &&
		(alpha == other.alpha) &&
		(addAlpha == other.addAlpha) &&
		(time == other.time) &&
		(isPlay == other.isPlay);
}
