#pragma once
#include "LeverComponent.h"

void LeverComponent::start()
{
	m_State = false; //set state to off on start
}

void LeverComponent::Interact()
{
	//override lever functionality
}
