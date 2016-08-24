#pragma once

#include "Vector2D.hpp"

class Texture
{
public:
	Texture::Texture(char* fileName);
	~Texture();
	void Draw(const double& X, const double& Y, const bool& TransFlag);
	void DrawRota(const double& X, const double& Y, const double& Exrate, const double& Angle, const bool& TransFlag, const bool& TurnFlag = true);
	void GetSize(int* width, int* height);

	Vector2D pos;

private:
	Texture(){}

	// グラフィックハンドル
	int gh;

};