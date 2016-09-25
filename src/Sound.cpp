#include "Sound.hpp"
#include "Graphics2D.hpp"
#include <DxLib.h>
#include <cassert>

constexpr double SoundFadeoutTime = 180.;


bool Sound::isLoaded = false;
std::vector<Sound::SoundFile*> Sound::mSound;


void Sound::Update()
{
	Update_fadeout();

#ifdef _DEBUG
	/* check num of playing sound mem*/
	int c_play = 0;
	for (auto i : mSound)
	{
		if (CheckSoundMem(i->handle) == TRUE)
			++c_play;
	}
#endif
}


void Sound::Update_fadeout()
{
	for (auto sound : mSound)
	{
		if (CheckSoundMem(sound->handle) == TRUE && sound->isFadeOut)
		{
			sound->volume -= sound->Max_vol / SoundFadeoutTime;
			if (sound->volume <= 0.)
			{
				StopSoundMem(sound->handle);
				sound->SetVolume(sound->Max_vol);
				sound->isFadeOut = false;
			}
			sound->SetVolume(sound->volume);
			break;
		}
	}
}


void Sound::Load()
{
	if(isLoaded)
	{
		assert(!"You are about to load again");
		return;
	}

	int tmpGr = LoadGraph("GRAPH/MENU/title.png");

	SetUseASyncLoadFlag(TRUE);

	isLoaded = true;
	
	mSound.reserve(static_cast<int>(eSound::eSoundNum));

	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/s1.mp3"), 170)); // opening
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/s2.mp3"), 170)); // 1
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/bossA.mp3"), 170)); // 2
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/bossA.mp3"), 170)); // 3
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/bossA.mp3"), 170)); // 4
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/bossA.mp3"), 170)); // 5
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/bossA.mp3"), 170)); // 6
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/bossA.mp3"), 170)); // 0
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/bossA.mp3"), 170)); // A
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/bossB.mp3"), 170)); // B
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/bossA.mp3"), 170)); // C
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/bossA.mp3"), 170)); // D
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/bossA.mp3"), 170)); // E
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/bossA.mp3"), 170)); // F
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/bossA.mp3"), 170)); // G
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/bossA.mp3"), 170)); // H

	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/bossA.mp3"), 170)); // gameover
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/bossA.mp3"), 170)); // nameentry

	assert(mSound.size() == static_cast<int>(eSound::eSoundNum) && "num of sound file not goes well with mSound.size()");

	bool finishFlag = false;
	int c_load = 0;
	while (!finishFlag)
	{
		ProcessMessage();
		++c_load;

		int i = 0;
		for (auto sound : mSound)
		{
			// finish
			if (CheckHandleASyncLoad(sound->handle) != FALSE)
				continue;
			finishFlag = true;
		}

		ClearDrawScreen();
		DrawRotaGraph(320, 240, 1., 0., tmpGr, TRUE);
		Sleep(1);
	}

	SetUseASyncLoadFlag(FALSE);
	DeleteGraph(tmpGr);
}


void Sound::Delete()
{
	for (auto i : mSound)
		DeleteSoundMem(i->handle);
	for (auto i : mSound)
		delete i;
	mSound.clear();
	isLoaded = false;
}


void Sound::Play(eSound sound)
{
	//ChangeVolumeSoundMem(mSound[static_cast<int>(sound)]->Max_vol, mSound[static_cast<int>(sound)]->handle);
	PlaySoundMem(mSound[static_cast<int>(sound)]->handle, DX_PLAYTYPE_LOOP);
	printfDx("ok");
}


void Sound::Stop()
{
	for (auto sound : mSound)
	{
		if (CheckSoundMem(sound->handle) == TRUE)
		{
			StopSoundMem(sound->handle);
			break;
		}
	}
}


void Sound::FadeOutStop()
{
	for (auto sound : mSound)
	{
		if (CheckSoundMem(sound->handle) == TRUE && sound->isFadeOut == false)
		{
			sound->isFadeOut = true;
			break;
		}
	}
}


void Sound::SetVolume(eSound sound, int volume)
{
	//mSound[static_cast<int>(sound)]->volume = volume;
	//ChangeVolumeSoundMem(volume, mSound[static_cast<int>(sound)]->handle);
	printfDx("Sorry, please don't use this func\n");
}


bool Sound::IsPlaySome()
{
	for (auto i : mSound)
	{
		if (CheckSoundMem(i->handle) == TRUE)
			return true;
	}

	return false;
}


Sound::SoundFile::SoundFile(int handle, double Max_vol)
	: Max_vol(Max_vol)
{
	this->handle = handle;
	volume = Max_vol;
	isFadeOut = false;
}


void Sound::SoundFile::SetVolume(int volumePal)
{
	volume = volumePal;
	ChangeVolumeSoundMem(volume, handle);
}
