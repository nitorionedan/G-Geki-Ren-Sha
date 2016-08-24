#include "DxLib.h"

#include "Texture.hpp"


Texture::Texture(char * fileName)
{
	LoadGraph(fileName);
}


Texture::~Texture()
{
	DeleteGraph(gh);
}


void Texture::Draw(const double & X, const double & Y, const bool & TransFlag)
{
	pos.SetVecor2D(X, Y);
	DrawGraph(pos.x, pos.y, gh, TransFlag);
}


void Texture::DrawRota(const double & X, const double & Y, const double & Exrate, const double & Angle, const bool & TransFlag, const bool & TurnFlag)
{
	pos.SetVecor2D(X, Y);
	DrawRotaGraph(pos.x, pos.y, Exrate, Angle, gh, TransFlag, TurnFlag);
}


void Texture::GetSize(int* width, int* height)
{
	GetGraphSize(gh, width, height);
}