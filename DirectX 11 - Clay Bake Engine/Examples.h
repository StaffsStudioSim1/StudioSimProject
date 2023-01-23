#pragma once
#include "Input/PlayerInput.h"
#include "Audio/SoundEffect.h"
class Examples
{
public:
	PlayerInput* playerInput;
	SoundEffect* soundEffect;

	Examples();
	~Examples();

	void Update();
};

