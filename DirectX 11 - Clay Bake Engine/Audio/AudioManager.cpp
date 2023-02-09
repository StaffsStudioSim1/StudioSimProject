#include "AudioManager.h"
#include "SoundEffect.h"

AudioManager::AudioManager()
{
	DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_Default;
#ifdef _DEBUG
	eflags |= DirectX::AudioEngine_Debug;
#endif
	_audioEngine = std::make_unique<DirectX::AudioEngine>(eflags);

	_retryAudio = false;
}

AudioManager::~AudioManager()
{
	if (_audioEngine)
		_audioEngine->Suspend();

	for (auto sound : _loopingSounds)
		sound->ResetLooping();

	_loopingSounds.clear();
}

void AudioManager::Update()
{
	if (_retryAudio)
	{
		_retryAudio = false;
		if (_audioEngine->Reset())
		{
			for (SoundEffect* sound : _loopingSounds)
				sound->Play();
		}
	}
	else if (!_audioEngine->Update())
	{
		// Audio device has been lost
		if (_audioEngine->IsCriticalError())
			_retryAudio = true;
	}
}

void AudioManager::SetMasterVolume(int volume)
{
	_audioEngine->SetMasterVolume(volume);
}

void AudioManager::AddLoopingSound(SoundEffect* sound)
{
	if (IsLooping(sound))
		return;

	_loopingSounds.push_back(sound);
}

void AudioManager::RemoveLoopingSound(SoundEffect* sound)
{
	if (!IsLooping(sound))
		return;

	_loopingSounds.erase(std::remove(_loopingSounds.begin(), _loopingSounds.end(), sound), _loopingSounds.end());
}

bool AudioManager::IsLooping(SoundEffect* sound)
{
	return std::count(_loopingSounds.begin(), _loopingSounds.end(), sound);
}

std::unique_ptr<DirectX::SoundEffect> AudioManager::GetSoundEffect(std::string filePath)
{
	return std::make_unique<DirectX::SoundEffect>(_audioEngine.get(), std::wstring(filePath.begin(), filePath.end()).c_str());
}
