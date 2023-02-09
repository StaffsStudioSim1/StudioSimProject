#pragma once
#include "Interactable.h"

class ButtonComponent : public Interactable
{
public:
	float m_Timer;
	float m_TimeLimit;
	bool m_Active;

	void Interact();
	void Update();
	void Start();

	ButtonComponent(InteractableLink switchType = Default, std::string linkedObjectName = "");
		
};

