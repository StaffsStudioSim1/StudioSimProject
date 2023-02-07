#pragma once
#include "Interactable.h"
#include "Physics.h"

class PressurePlateComponent : public Interactable
{
public:
	bool m_WeighedDown;
	void start();
	void WeighedDown();
	void WeightReleased();
	void Update();
	

	// add physics hitbox

};
