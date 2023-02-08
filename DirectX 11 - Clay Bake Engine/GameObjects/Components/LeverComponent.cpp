#pragma once
#include "LeverComponent.h"
#include "../ObjectHandler.h"

void LeverComponent::start()
{
	m_State = false; //set state to off on start
}

void LeverComponent::Interact()
{
	//override lever functionality
	SendSignal();
}

LeverComponent::LeverComponent(InteractableLink switchType, std::string linkedObjectName)
{
	interactableLink = switchType;

	if (linkedObjectName != "")
	{
		linkedObject = ObjectHandler::GetInstance().FindGameObject(linkedObjectName);
	}
}


