#pragma once
#include "Interactable.h"

class LeverComponent : public Interactable
{
public:
	bool m_State;
	std::string destinationObject;
	void start();
	void Interact();

	LeverComponent(InteractableLink switchType = Default, std::string linkedObjectName = "");
};
  
