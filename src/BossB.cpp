#include "BossB.hpp"
#include <DxLib.h>


BossB::BossB()
	: body_head(new Body(ePart::head))
	, body_leftWing(new Body(ePart::leftWing))
	, body_rightWing(new Body(ePart::rightWing))
	, body_tail(new Body(ePart::tail))
{
}


BossB::~BossB()
{
}


void BossB::Update()
{
}


void BossB::Draw()
{
}


bool BossB::HitCheck(const double & ColX, const double & ColY, const int & DamagePoint)
{
	return false;
}


const bool BossB::IsDead() const
{
	return false;
}

BossB::Body::Body(ePart part)
{
}

BossB::Body::~Body()
{
}

void BossB::Body::Update()
{
}

void BossB::Body::Draw()
{
}

void BossB::Body::Initialize()
{
}
