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


void Shot::Update(const double& PosX, const double& PosY){
	mShot->Update(PosX, PosY);
}


void Shot::Draw(){
	mShot->Draw();
}


void Shot::Fire(const double & SPEED, const double & ANGLE){
	mShot->Fire(SPEED, ANGLE);
}


void Shot::Fire(const double & PosX, const double & PosY, const double & SPEED, const double & ANGLE)
{
	mShot->Fire(PosX, PosY, SPEED, ANGLE);
}


bool Shot::IsHit(const double & ColX, const double & ColY, const double & ColR){
	return mShot->IsHit(ColX, ColY, ColR);
}

// EOF