#include "Character.hpp"
#include "Eshot.hpp"
#include "NullTask.hpp"
#include "BossA.hpp"


Character::Character(Task* task)
	: mTask(task)
{
}

Character::~Character(){
	delete mTask;
}

void Character::Update(){
	mTask->Update();
}

void Character::Draw(){
	mTask->Draw();
}


void Character::ChangeShot(eChara chara)
{
	delete mTask;
	switch (chara)
	{
	case eChara_ShotA :
		mTask = (Task*) new Eshot;
		break;
	case eChara_None :
		mTask = (Task*) new NullTask;
		break;
	}
}
// EOF