#pragma once

#include "Task.hpp"

/// nullptr‚Ì‘ã‚í‚è
class NullTask : public Task
{
public:
	NullTask(){}
	~NullTask(){}
	virtual void Update() override {}	// ‰½‚à‚µ‚È‚¢
	virtual void Draw() override {}		// ‰½‚à‚µ‚È‚¢
};