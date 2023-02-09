#pragma once
#include <Audio.h>
class SoundEffect
{
public:
	SoundEffect(std::string filePath, bool loop = false);
	~SoundEffect();

	void Play();
	void Stop();

	void SetVolume(float volume);
	void SetPitch(float pitch);
	void SetPan(float pan);

	// Called by AudioManager - do not use
	void ResetLooping();
private:
	std::unique_ptr<DirectX::SoundEffect> _sfx;
	std::unique_ptr<DirectX::SoundEffectInstance> _loopingSfx;

	bool _loop = false;
	float _volume = 1.0f;
	float _pitch = 1.0f;
	float _pan = 0.0f;
};

