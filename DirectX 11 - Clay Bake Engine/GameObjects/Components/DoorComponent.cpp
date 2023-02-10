#pragma once
#include "DoorComponent.h"
#include "../GameObject.h"
#include "AABB.h"
#include "Appearance.h"
#include "../../Defines.h"

void DoorComponent::Start()
{
	m_Opened = false;
#if !EDIT_MODE
	_gameObject->GetTransform()->SetPosition(_gameObject->GetTransform()->GetPosition() + Vector2(0.0f, 7.0f));
#endif
}

json DoorComponent::Write()
{
	json me;
	me[JSON_COMPONENT_CLASS] = "DoorComponent";
	return me;
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