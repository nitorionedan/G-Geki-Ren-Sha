#pragma once
#include <vector>


enum class eSound
{
	stage1,
	stage2,
	stage3,
	stage4,
	stage5,
	stage6,
	stage0,
};


class Sound
{
public:
	static void Load();
	static void Delete();
	static void Play(eSound sound);
	static void Stop();
	static void FadeOut();
	static void SetVolume(eSound sound);

private:
	Sound() {}
	~Sound() {}

	// @brief ‰¹Šy‚ÌØ‚è‘Ö‚¦i‚Ô‚ÂØ‚è–h~j
	static void Change();

	static std::vector<int> h_sound;
	static bool isLoaded;
};

// GetCurrentPositionSoundMem