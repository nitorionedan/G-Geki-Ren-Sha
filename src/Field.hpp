#pragma once

class Field
{
public:
	Field(){}
	virtual ~Field(){}
	virtual void Update() = 0;
	virtual void Draw() = 0;
};