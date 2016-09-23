#pragma once
#include <vector>


enum class eSound
{
	opening,
	stage1,
	stage2,
	stage3,
	stage4,
	stage5,
	stage6,
	stage0,
	bossA,
	bossB,
	bossC,
	bossD,
	bossE,
	bossF,
	bossG,
	bossH,
	gameover,
	name_entry,
};


class Sound
{
	class SoundFile 
	{
	public:
		SoundFile(int handle)
		{
			this->handle = handle;
			volume = 255;
		}
		~SoundFile() {}

		int volume;
		int handle;
	};

public:
	static void Load();
	static void Delete();
	static void Play(eSound sound);
	static void Stop();
	static void FadeOut();
	static void SetVolume(eSound sound, int volume);

private:
	Sound() {}
	~Sound() {}

	// @brief ‰¹Šy‚ÌØ‚è‘Ö‚¦i‚Ô‚ÂØ‚è–h~j
	static void Change();

	static std::vector<SoundFile> mSound;
	static bool isLoaded;
};

// GetCurrentPositionSoundMem