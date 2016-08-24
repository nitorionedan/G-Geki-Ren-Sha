#include "DxLib.h"

#include "Particle.hpp"
#include <time.h>


Particle::Particle(int ParticleLife) : c_life( new Counter(ParticleLife) )
{
	x_pos   = static_cast<double>( GetRand(640) );
	y_pos   = static_cast<double>( GetRand(480) );
	speed   = static_cast<double>( GetRand(100) ) / 100.0;		// 0.00Å`1.00
	ang_spd = static_cast<double>( GetRand(100) ) / 1000.0;		// 0.000Å`0.001
	f_dead  = false;
}


Particle::~Particle()
{}


void Particle::Update()
{
	c_life->Update();
	angle -= ang_spd;						// âÒì]
	y_pos += speed;							// óéâ∫
	if (c_life->isLast())	f_dead = true;
}


void Particle::Draw(int &Gr_Handle)
{
	DrawRotaGraph(x_pos, y_pos, 1.0, angle, Gr_Handle, true);
}


bool Particle::isDead() {
	return f_dead;
}