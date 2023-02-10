#pragma once
#include "Interactable.h"

class LeverComponent : public Interactable
{
public:
	bool state;
	std::string destinationObject;
	void Start();
	void Interact();
	json Write();

	LeverComponent(InteractableLink switchType, std::string linkedObjectName);
};
  
