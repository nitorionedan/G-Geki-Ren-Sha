#pragma once

#include "Eshot.hpp"

#include <memory>
#include <array>


class EshotMng
{
public:
	EshotMng();
	~EshotMng();

	void Update(const Player& player);
	void Draw();

private:
	std::unique_ptr<Eshot>	eshot;
};