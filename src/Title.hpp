#pragma once


// タイトル画面クラス
class Title
{
public:
	const double Y_MIN;

public:
	int		hg_title;
	int		hs_intro;
	double	x, y;
	double	speed;

	bool	isStop;

public:
	void Update();
	void Draw();

	Title();
	~Title();
};