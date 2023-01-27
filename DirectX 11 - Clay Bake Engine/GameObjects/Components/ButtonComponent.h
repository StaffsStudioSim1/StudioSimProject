#pragma once
#include "Interactable.h"

class ButtonComponent : public Interactable
{
public:
	float m_Timer;
	bool m_Opened;
	void Interact();
	void Start();
};

