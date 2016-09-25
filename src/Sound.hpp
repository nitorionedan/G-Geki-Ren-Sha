#pragma once
#include <vector>
#include <memory>


enum class eSound : int
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

	eSoundNum,
};


class Sound
{
	class SoundFile 
	{
	public:
		SoundFile(int handle, double Max_vol);
		~SoundFile() {}
		void SetVolume(int volumePal);

		const double Max_vol;
		double volume;
		int handle;
		bool isFadeOut;

	private:
		SoundFile() : Max_vol(255) {}
	};

public:
	static void Update();
	static void Update_fadeout();
	static void Load();
	static void Delete();
	static void Play(eSound sound);
	static void Stop();
	static void FadeOutStop();
	static int GetVolume(eSound sound) {
		return mSound[static_cast<int>(sound)]->volume;
	}
	static bool IsPlaySome();

private:
	Sound() {}
	~Sound() {}
	static void SetVolume(eSound sound, int volume);

	static std::vector<SoundFile*> mSound;
	static bool isLoaded;
};

// GetCurrentPositionSoundMem