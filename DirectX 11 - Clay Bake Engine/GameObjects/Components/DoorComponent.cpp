#pragma once
#include "DoorComponent.h"

void DoorComponent::start()
{
	m_Opened = false;
}

void DoorComponent::Open()
{
	//disable renderer if enabled
	_gameObject->GetComponent<Appearance>()->SetAlphaMultiplier(0.0f);


	//disable collider if enabled
	if (_gameObject->GetComponent<AABB>()->IsActive())
	{
		_gameObject->GetComponent<AABB>()->SetActive(false);
	}

	m_Opened = true;
}

void DoorComponent::Close()
{
	//TODO: enable renderer if disabled
	_gameObject->GetComponent<Appearance>()->SetAlphaMultiplier(1.0f);

	//disable collider if enabled
	if (!_gameObject->GetComponent<AABB>()->IsActive())
	{
		_gameObject->GetComponent<AABB>()->SetActive(true);
	}

	m_Opened = false;
}

DoorComponent::DoorComponent()
{
	start();
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