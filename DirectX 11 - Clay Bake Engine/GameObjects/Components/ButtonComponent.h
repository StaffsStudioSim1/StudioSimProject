#pragma once
#include "Interactable.h"

class ButtonComponent : public Interactable
{
public:
	int m_Timer;
	bool m_State;
	void Interact();
};

