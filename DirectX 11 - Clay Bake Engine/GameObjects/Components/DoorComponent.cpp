#pragma once
#include "DoorComponent.h"

void DoorComponent::start()
{
	m_Opened = false;
}

void DoorComponent::Open()
{
	//TODO: disable renderer if enabled

	//TODO: disable collider if enabled

	m_Opened = true;
}

void DoorComponent::Close()
{
	//TODO: enable renderer if disabled

	//TODO: enable collider if disabled

	m_Opened = false;
}

DoorComponent::DoorComponent()
{
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