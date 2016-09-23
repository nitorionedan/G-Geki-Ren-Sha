#include "Sound.hpp"
#include <DxLib.h>
#include <cassert>


bool Sound::isLoaded = false;
std::vector<Sound::SoundFile> Sound::mSound;


void Sound::Load()
{
	if(isLoaded)
	{
		assert(!"You are about to load again");
		return;
	}

	isLoaded = true;
	
	mSound.reserve(10);
	mSound.push_back(SoundFile(LoadSoundMem("SOUND/s1.mp3"))); // opening
	mSound.push_back(SoundFile(LoadSoundMem("SOUND/s2.mp3"))); // 1
	mSound.push_back(SoundFile(LoadSoundMem(NULL))); // 2
	mSound.push_back(SoundFile(LoadSoundMem(NULL))); // 3
	mSound.push_back(SoundFile(LoadSoundMem(NULL))); // 4
	mSound.push_back(SoundFile(LoadSoundMem(NULL))); // 5
	mSound.push_back(SoundFile(LoadSoundMem(NULL))); // 6
	mSound.push_back(SoundFile(LoadSoundMem(NULL))); // 0

	mSound.push_back(SoundFile(LoadSoundMem("bossA.mp3"))); // A
	mSound.push_back(SoundFile(LoadSoundMem("bossA.mp3"))); // B
	mSound.push_back(SoundFile(LoadSoundMem("bossA.mp3"))); // C
	mSound.push_back(SoundFile(LoadSoundMem("bossA.mp3"))); // D
	mSound.push_back(SoundFile(LoadSoundMem("bossA.mp3"))); // E
	mSound.push_back(SoundFile(LoadSoundMem("bossA.mp3"))); // F
	mSound.push_back(SoundFile(LoadSoundMem("bossA.mp3"))); // G
	mSound.push_back(SoundFile(LoadSoundMem("bossA.mp3"))); // H
}


void Sound::Delete()
{
	for (auto i : mSound)
		DeleteSoundMem(i.handle);
	mSound.clear();
	isLoaded = false;
}


void Sound::Play(eSound sound)
{
	bool isPlay = false;

	for(auto i : mSound)
	{
		if (CheckSoundMem(i.handle) == FALSE)
			continue;
		StopSoundMem(i.handle);
		isPlay = true;
	}

	PlaySoundMem(mSound.at(static_cast<int>(sound)).handle, DX_PLAYTYPE_LOOP);
}


void Sound::Stop()
{
}


void Sound::FadeOut()
{
}


void Sound::SetVolume(eSound sound, int volume)
{
	mSound[static_cast<int>(sound)].volume = volume;
	ChangeVolumeSoundMem(volume, mSound[static_cast<int>(sound)].handle);
}


void Sound::Change()
{
}
