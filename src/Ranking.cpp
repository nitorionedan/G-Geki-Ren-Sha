#include "Ranking.hpp"
#include "FileDef.h"
#include "Graphics2D.hpp"
#include <DxLib.h>


Ranking::Ranking(ISceneChanger * changer)
	: BaseScene(changer)
{
}


void Ranking::Initialize()
{
	int h_file = NULL;

	h_file = FileRead_open(MyFile::Dat::RANKING);



	FileRead_close(h_file);
}


void Ranking::Update()
{
}


void Ranking::Draw()
{
}
