#pragma once
#include "Interactable.h"

class LeverComponent : public Interactable
{
public:
	bool m_State;
	std::string destinationObject;
	void Start() override;
	void Interact();
	json Write() override;

	LeverComponent(InteractableLink switchType, std::string linkedObjectName);
};
  
