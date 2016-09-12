#pragma once
#include "Field.hpp"


class NullStage : public Field
{
public:
	NullStage() {}
	~NullStage() {}
	virtual void Update() override {}
	virtual void Draw() override {}
};