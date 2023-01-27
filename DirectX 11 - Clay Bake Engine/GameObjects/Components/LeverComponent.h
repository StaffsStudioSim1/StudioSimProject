#pragma once
#include "../Components/Interactable.h"
class LeverComponent : public Interactable
{
public:
	bool m_State;
	void start();
	void Interact();
};
  
