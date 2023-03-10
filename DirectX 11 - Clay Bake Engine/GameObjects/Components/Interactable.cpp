#pragma once
#include "Interactable.h"
#include "DoorComponent.h"
#include "../ObjectHandler.h"


void Interactable::Start()
{

	if (linkedObjectName != "")
	{
		linkedObject = ObjectHandler::GetInstance().FindGameObject(linkedObjectName);
	}
}

void Interactable::Interact()
{
	//implementation in derived classe
}

void Interactable::SendSignal()
{
	
	switch (interactableLink)
	{
	case (Default):
		//do nothing
		break;
	case (SwitchState):
		//call switch state on linked object ie door
	
		if (linkedObject->GetComponent<DoorComponent>())
		{
			linkedObject->GetComponent<DoorComponent>()->SwitchState();
		}

		break;
	case (SwitchGravity):
		//switch gravity

		GameManager().GetInstance().GravityFlip();

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
Interactable::Interactable(InteractableLink switchType, std::string linkedObjectName)
{
	interactableLink = switchType;
	this->linkedObjectName = linkedObjectName;
	
}

//define InteractableLink switchType to define what signal, without a target object 
Interactable::Interactable(InteractableLink switchType)
{
	interactableLink = switchType;
}