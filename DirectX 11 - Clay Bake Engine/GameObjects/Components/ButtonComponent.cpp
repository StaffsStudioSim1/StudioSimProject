#include "ButtonComponent.h"

void ButtonComponent::Start()
{
	m_Active= false; //set state to off on start
	m_Timer = 0;
}

//ButtonComponent(InteractableLink switchType, std::string linkedObject, float timeLimit)
//{
//	interactableLink = switchType;
//	linkedObjectName = linkedObject;
//
//	m_TimeLimit = timeLimit;
//}

void ButtonComponent::Interact()
{
	//Button funcitonality implemented here
	m_Active = true;
	SendSignal();
}

void Update()
{
	//increase timer if button pressed
	//resend signal when time elapsed and reset
}