#include "EneShotCreater.hpp"
#include "Player.hpp"
#include "HitEffect.hpp"
#include "Bomb.hpp"
#include <DxLib.h>
#include <cmath>
#include <algorithm>
#include <cassert>

const double EneShotCreater::BD_LEFT = -10.;
const double EneShotCreater::BD_RIGHT = 660.;
const double EneShotCreater::BD_TOP = BD_LEFT;
const double EneShotCreater::BD_BOTTOM = 500.;


EneShotCreater::EneShotCreater()
{

}


EneShotCreater::~EneShotCreater()
{

}


void EneShotCreater::Update()
{

}


void EneShotCreater::Draw()
{

}


void EneShotCreater::Fire(eEneShot shotType, eEneShotMove moveType, Vector2D& pos, double& speed)
{
}


// ===========================================================
std::shared_ptr<EneShotCreater> IEneShotCreater::mEShotCreater;


void IEneShotCreater::set(std::shared_ptr<EneShotCreater> eneShotCreater)
{
	mEShotCreater = eneShotCreater;
}


void IEneShotCreater::Fire(eEneShot shotType, eEneShotMove moveType, Vector2D & firstPos, double & speed)
{
	mEShotCreater->Fire(shotType, moveType, firstPos, speed);
}
