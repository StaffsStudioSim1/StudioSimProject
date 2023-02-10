#pragma once
#include "DoorComponent.h"
#include "../GameObject.h"
#include "AABB.h"
#include "Appearance.h"
#include "../../Defines.h"

void DoorComponent::Start()
{
	_open = _defaultState;

	if (_open)
	{
		Open();
	}
	if (!_open)
	{
		Close();
	}

#if !EDIT_MODE
	_gameObject->GetTransform()->SetPosition(_gameObject->GetTransform()->GetPosition() + Vector2(0.0f, 7.0f));
#endif
}

json DoorComponent::Write()
{
	json me;
	me[JSON_COMPONENT_CLASS] = "DoorComponent";
	me[JSON_COMPONENT_CONSTRUCTORS].push_back(_defaultState);
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

	_open = true;
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

	_open = false;
}

DoorComponent::DoorComponent(bool defaultState)
{
	_defaultState = defaultState;
}


void DoorComponent::SwitchState()
{
	if (_open)
	{
		Close();
	}
	else if (!_open)
	{
		Open();
	}
}