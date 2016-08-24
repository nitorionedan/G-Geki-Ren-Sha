#include "DxLib.h"

#include "Effect.hpp"


Effect::Effect(EffectTask* EFtask)
	: mEFTask(EFtask)
{
}


Effect::~Effect(){
	delete mEFTask;
}


void Effect::Update(){
	mEFTask->Update();
}


void Effect::Update(const double& X, const double& Y){
	mEFTask->Update(X, Y);
}


void Effect::Draw(){
	mEFTask->Draw();
}


void Effect::PlayAnime(const double & MyX, const double & MyY){
	mEFTask->PlayAnime(MyX, MyY);
}
// EOF