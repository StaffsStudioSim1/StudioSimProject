#include "PressurePlateComponent.h"
#include "../ObjectHandler.h"

PressurePlateComponent::PressurePlateComponent(InteractableLink switchType, std::string linkedObjectName)
{
	interactableLink = switchType;

	if (linkedObjectName != "")
	{
		linkedObject = ObjectHandler::GetInstance().FindGameObject(linkedObjectName);
	}
}

void PressurePlateComponent::start()
{
	
	m_WeighedDown = false;
}

void PressurePlateComponent::WeighedDown()
{
	SendSignal();
	m_WeighedDown = true;
}

void PressurePlateComponent::WeightReleased()
{
	SendSignal();
	m_WeighedDown = false;
}

void PressurePlateComponent::Update()
{
	//check hitbox state
	//physics trigger volume
	// if volume occupied call weighed down, call weight released on volume not being occupied
	
}

