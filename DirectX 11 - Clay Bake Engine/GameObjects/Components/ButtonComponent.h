#pragma once
#include "Interactable.h"

class ButtonComponent : public Interactable
{
public:
	float m_Timer;
	float m_TimeLimit;
	bool m_Active;
	void Interact();
	void Start();

	ButtonComponent(InteractableLink switchType = Default, std::string linkedObjectName = "");

	//TODO: finish button component constructor
	// ButtonComponent(InteractableLink, std::string, float);
};

