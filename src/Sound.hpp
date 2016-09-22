#pragma once


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
	static void SetVolume(eSound sound);

private:
	Sound() {}
	~Sound() {}

	// @brief ���y�̐؂�ւ��i�Ԃ؂�h�~�j
	static void Change();

	static bool isLoaded;
};