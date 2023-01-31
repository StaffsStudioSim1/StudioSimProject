#pragma once
#include "../GameObject.h"
#include "Component.h"

class Interactable: public Component
{

	enum InteractableLink
	{
		Default = 0,
		DoorOpen = 1,
		DoorClose = 2,
		SwitchGravity = 3
	};
public: 
	InteractableLink interactableLink; //InteractableLink chooses what the function of each interactable is
	 virtual void Interact(); //virtual method for interacting

	Interactable(InteractableLink switchType = Default, std::string linkedObject = "");


	 //virtual void Update();
};

