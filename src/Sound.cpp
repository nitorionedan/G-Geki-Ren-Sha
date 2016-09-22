#include "Sound.hpp"
#include <DxLib.h>
#include <cassert>


bool Sound::isLoaded = false;
std::vector<int> Sound::h_sound;


void Sound::Load()
{
	if(isLoaded)
	{
		assert(!"You try to load again");
		return;
	}

	isLoaded = true;
	
	h_sound.reserve(10);
	h_sound.push_back(LoadSoundMem("SOUND/s0.mp3"));
	h_sound.push_back(LoadSoundMem("SOUND/s1.mp3"));
	h_sound.push_back(LoadSoundMem("SOUND/s2.mp3"));
}


void Sound::Delete()
{
	for (auto i : h_sound)
		DeleteSoundMem(i);
	h_sound.clear();
	isLoaded = false;
}


void Sound::Play(eSound sound)
{
	bool isPlay = false;

	for(auto i : h_sound)
	{
		if (CheckSoundMem(i) == FALSE)
			continue;
		
		StopSoundMem(i);
		isPlay = true;
	}

	switch (sound)
	{
	case eSound::stage1:
		PlaySoundMem(h_sound.at(0), DX_PLAYTYPE_LOOP);
		break;
	case eSound::stage2:
		PlaySoundMem(h_sound.at(1), DX_PLAYTYPE_LOOP);
		break;
	case eSound::stage3:
		PlaySoundMem(h_sound.at(2), DX_PLAYTYPE_LOOP);
		break;
	case eSound::stage4:
		break;
	case eSound::stage5:
		break;
	case eSound::stage6:
		break;
	case eSound::stage0:
		break;
	}
}


void Sound::Stop()
{
}


void Sound::FadeOut()
{
}


void Sound::SetVolume(eSound sound)
{
	switch (sound)
	{
	case eSound::stage1:
		ChangeVolumeSoundMem(170, h_sound[0]); // 170
		break;
	case eSound::stage2:
		ChangeVolumeSoundMem(170, h_sound[1]);
		break;
	case eSound::stage3:
		ChangeVolumeSoundMem(170, h_sound[2]);
		break;
	case eSound::stage4:
		break;
	case eSound::stage5:
		break;
	case eSound::stage6:
		break;
	case eSound::stage0:
		break;
	}
}


void Sound::Change()
{
}
