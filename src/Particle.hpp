#pragma once
#include "Counter.hpp"
#include <memory>


class Particle
{
private:
	std::unique_ptr<Counter> c_life;
	
	double x_pos, y_pos;	// À•W
	double speed;			// —‚¿‚é‘¬‚³
	double ang_spd;			// ‰ñ“]‚Ì‘¬‚³
	double angle;			// ‰ñ“]’l
	bool   f_dead;

public:
	Particle(int PartcleLife);
	~Particle();
	void Update();
	void Draw(int &Gr_Handle);
	bool isDead();
};