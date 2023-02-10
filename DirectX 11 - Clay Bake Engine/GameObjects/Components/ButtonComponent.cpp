#include "ButtonComponent.h"
#include "../ObjectHandler.h"

void ButtonComponent::Start()
{
	Interactable::Start();
	_active= false; //set state to off on start
	_timer = 0;
	_timeLimit = 100;
}

ButtonComponent::ButtonComponent(InteractableLink switchType, std::string linkedObjectName) : Interactable(switchType, linkedObjectName)
{
}

void ButtonComponent::Interact()
{
	//Button funcitonality implemented here
	_active = true;
	SendSignal();
}

void ButtonComponent::Update(float deltaTime)
{
	if (_active)
	{
		_timer += 1;
	}

	if (_timer >= _timeLimit)
	{
		SendSignal();
		_active = false;
	}
}