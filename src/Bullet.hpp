#pragma once


class Bullet
{
public:
	Bullet();
	~Bullet();
	void Set(const double X_POS, const double Y_POS);

	int atk;
	double x_pos, y_pos;
	double width, height;
	double angle;
	double speed;
	bool   f_exist;
};