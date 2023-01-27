#pragma once
#include "Interactable.h"

class ButtonComponent : public Interactable
{
public:
	float m_Timer;
	bool m_State;
	void Interact();
	void Start();
};

