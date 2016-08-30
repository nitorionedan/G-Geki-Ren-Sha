#include "DxLib.h"

#include "EshotMng.hpp" // BossA
#include "Game.hpp"


EshotMng::EshotMng()
	: eshot(new Eshot)
{
}


EshotMng::~EshotMng()
{
}


void EshotMng::Update(const Player& player)
{
	eshot->Update(player);
}


void EshotMng::Draw()
{
	eshot->Draw();
}
