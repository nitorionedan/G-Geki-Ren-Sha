#include "DxLib.h"

#include "Texture.hpp"
#include <cassert>

Texture::Texture(char * fileName)
{
	int tmp = LoadGraph(fileName);
	assert(tmp != -1 && tmp != 0 && "Texture::Texture()");
}


Texture::~Texture()
{
	DeleteGraph(gh);
}


void Texture::Draw(const double & X, const double & Y, const bool & TransFlag)
{
	pos.SetVec(X, Y);
	DrawGraph(pos.x, pos.y, gh, TransFlag);
}


void Texture::DrawRota(const double & X, const double & Y, const double & Exrate, const double & Angle, const bool & TransFlag, const bool & TurnFlag)
{
	pos.SetVec(X, Y);
	DrawRotaGraph(pos.x, pos.y, Exrate, Angle, gh, TransFlag, TurnFlag);
}


void Texture::GetSize(int* width, int* height)
{
	GetGraphSize(gh, width, height);
}