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

	// @brief 音楽の切り替え（ぶつ切り防止）
	static void Change();

	static bool isLoaded;
};