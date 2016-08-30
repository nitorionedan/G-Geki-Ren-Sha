#include "Shot.hpp"


const double Shot::SC_LIMIT_XL = -10.;
const double Shot::SC_LIMIT_XR = 650.;
const double Shot::SC_LIMIT_YT = -20.;
const double Shot::SC_LIMIT_YB = 490.;


Shot::Shot(ShotTask * shot)
	: mShot(shot)
{
}


Shot::~Shot(){
	delete mShot;
}