#pragma once



class Counter2
{
public:
	Counter2();
	~Counter2(){}

	void SetMax(const int& MAX_);
	void Update();
	void Reset();
	int GetCount();
	bool isLast();
	bool isEach(const int& MIN_, const int& MAX_);

private:
	int max;
	int count;
};

// EOF