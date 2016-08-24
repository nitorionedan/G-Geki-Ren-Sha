#pragma once
#include "Task.hpp"


class BackGround : public Task
{
public:
	void All();
	void Update() override;
	void Draw() override;
};