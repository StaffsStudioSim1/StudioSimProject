#pragma once
#include "LeverComponent.h"
#include "../ObjectHandler.h"

void LeverComponent::Start()
{
	Interactable::Start();
	m_State = false; //set state to off on start
}

void LeverComponent::Interact()
{
	//override lever functionality
	SendSignal();
}

LeverComponent::LeverComponent(InteractableLink switchType, std::string linkedObjectName) : Interactable(switchType, linkedObjectName)
{
}

json LeverComponent::Write()
{
	json me;
	me[JSON_COMPONENT_CLASS] = "LeverComponent";
	me[JSON_COMPONENT_CONSTRUCTORS].push_back(interactableLink);
	me[JSON_COMPONENT_CONSTRUCTORS].push_back(_linkedObjectName);
	return me;
}

