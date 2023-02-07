#pragma once
#include "Interactable.h"

void Interactable::Interact()
{
	//implementation in derived classe
}

void Interactable::SendSignal()
{
	//TODO: find linkedObject by name 
	
	switch (interactableLink)
	{
	case (Default):
		//do nothing
		break;
	case (SwitchState):
		//call switch state on linked object ie door
		

		break;
	case (SwitchGravity):
		//switch gravity


		break;
	case (Signal3):
		//spare signal

		break;
	case (signal4):
		//spare signal
		break;

	}


}

//define InteractableLink switchType to define what signal is sent and linkedObject of the name of the target object
Interactable::Interactable(InteractableLink switchType, std::string linkedObject)
{
	interactableLink = switchType;
	linkedObjectName = linkedObject;
}

//define InteractableLink switchType to define what signal, without a target object 
Interactable::Interactable(InteractableLink switchType)
{
	interactableLink = switchType;
}
