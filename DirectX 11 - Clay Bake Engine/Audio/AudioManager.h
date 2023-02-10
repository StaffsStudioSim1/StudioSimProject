#pragma once
#pragma comment(lib, "DirectXtk.lib")
#include <Audio.h>
class SoundEffect;
class AudioManager
{
public:
	// Singleton Code
	static AudioManager& GetInstance()
	{
		static AudioManager instance;
		return instance;
	}

	AudioManager(AudioManager const&) = delete;
	void operator=(AudioManager const&) = delete;

	void Kill();

	void Update();

	void SetMasterVolume(int volume);

	void AddLoopingSound(SoundEffect* sound);
	void RemoveLoopingSound(SoundEffect* sound);
	bool IsLooping(SoundEffect* sound);

	std::unique_ptr<DirectX::SoundEffect> GetSoundEffect(std::string filePath);
private:
	AudioManager();

	std::vector<SoundEffect*> _loopingSounds;
	std::unique_ptr<DirectX::AudioEngine> _audioEngine;
	bool _retryAudio;
};

