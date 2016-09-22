#include "Sound.hpp"
#include <DxLib.h>
#include <cassert>


bool Sound::isLoaded;


void Sound::Load()
{
	if(isLoaded == false)
	{
		assert(!"You try to load again");
	}

	isLoaded = true;
}


void Sound::Delete()
{
}

void Sound::Play(eSound sound)
{
}


void Sound::Stop()
{
}


void Sound::SetVolume(eSound sound)
{
}


void Sound::Change()
{
}
