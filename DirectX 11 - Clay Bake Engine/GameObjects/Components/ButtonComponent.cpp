#include "ButtonComponent.h"
#include "../ObjectHandler.h"

void ButtonComponent::Start()
{
	Interactable::Start();
	m_Active= false; //set state to off on start
	m_Timer = 0;
	m_TimeLimit = 100;
}

ButtonComponent::ButtonComponent(InteractableLink switchType, std::string linkedObjectName) : Interactable(switchType, linkedObjectName)
{
	interactableLink = switchType;

	if (linkedObjectName != "")
	{
		linkedObject = ObjectHandler::GetInstance().FindGameObject(linkedObjectName);
	}
}

void ButtonComponent::Interact()
{
	//Button funcitonality implemented here
	m_Active = true;
	SendSignal();
}

void ButtonComponent::Update()
{
	if (m_Active)
	{
		m_Timer += 1;
	}

	if (m_Timer >= m_TimeLimit)
	{
		SendSignal();
		m_Active = false;
	}
}