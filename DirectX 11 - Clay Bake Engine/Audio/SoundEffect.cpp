#include "SoundEffect.h"
#include "AudioManager.h"

SoundEffect::SoundEffect(std::string filePath, bool loop)
{
	_sfx = AudioManager::GetInstance().GetSoundEffect(filePath);
	_loop = loop;
	if (loop)
		_loopingSfx = _sfx->CreateInstance();
}

SoundEffect::~SoundEffect()
{
	if (_loop)
	{
		Stop();
		AudioManager::GetInstance().RemoveLoopingSound(this);
	}
}

void SoundEffect::Play()
{
	if (_loop)
	{
		AudioManager::GetInstance().AddLoopingSound(this);
		_loopingSfx->SetVolume(_volume);
		_loopingSfx->SetPitch(_pitch);
		_loopingSfx->SetPan(_pan);
		_loopingSfx->Play(true);
	}
	else
		_sfx->Play(_volume, _pitch, _pan);
}

void SoundEffect::Stop()
{
	if (_loop)
		_loopingSfx->Stop(true);
}

void SoundEffect::SetVolume(float volume)
{
	_volume = volume;
}

void SoundEffect::SetPitch(float pitch)
{
	_pitch = pitch;
}

void SoundEffect::SetPan(float pan)
{
	_pan = pan;
}

void SoundEffect::ResetLooping()
{
	if (_loop)
		_loopingSfx.reset();
}
