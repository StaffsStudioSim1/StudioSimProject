#pragma once
#include "DoorComponent.h"

void DoorComponent::start()
{
	m_Opened = false;
}

void DoorComponent::Interact()
{
	//doors usually aren't interactable and are instead opened by other events
}


void DoorComponent::Open()
{
	m_Opened = true;
}

void DoorComponent::Close()
{
	m_Opened = false;
}

void DoorComponent::SwitchState()
{
	if (m_Opened)
	{
		Close();
	}
	else if (!m_Opened)
	{
		Open();
	}
}