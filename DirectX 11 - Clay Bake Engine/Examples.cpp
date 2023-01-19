#include "Examples.h"
#include <debugapi.h>

Examples::Examples()
{
	// Instantiate the PlayerInput
	playerInput = new PlayerInput();
	// Make this PlayerInput use the left hand side of the keyboard
	playerInput->SetDeviceType(KeyboardLeft);

	// Create a SoundEffect with the specified file path
	soundEffect = new SoundEffect("Resources/Laser_Shoot3.wav");
}

Examples::~Examples()
{
	// Delete the PlayerInput
	delete playerInput;

	// Delete the SoundEffect
	delete soundEffect;
}

void Examples::Update()
{
	// Is jump pressed this frame?
	if (playerInput->IsActionDown(Jump))
		OutputDebugStringA("Jump Pressed\n");
		//soundEffect->Play();
	// Is jump being held?
	if (playerInput->IsActionHeld(Jump))
		OutputDebugStringA("Jump Held\n");
	// Was jump released?
	if (playerInput->IsActionUp(Jump))
		OutputDebugStringA("Jump Released\n");
}
