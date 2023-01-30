#pragma once
#include "../GameObject.h"
#include "Component.h"

class Interactable: public Component
{
public: 
	 virtual void Interact(); //virtual method for interacting 
	 //virtual void Update();
};