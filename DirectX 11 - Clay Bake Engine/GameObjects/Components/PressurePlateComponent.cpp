#include "PressurePlateComponent.h"

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

