#pragma once
#include "Interactable.h"
#include "../ObjectHandler.h"
#include "../PlayerController.h"
#include "../PlayerMagnetism.h"

class PressurePlateComponent : public Interactable
{
public:
	bool m_WeighedDown;
	Vector2 m_Boxsize;
	Vector2 m_Position;
	
	void WeighedDown();
	void WeightReleased();

	json Write();

	void Start();
	void Update();
	
	PressurePlateComponent(InteractableLink switchType = Default, std::string linkedObjectName = "");
	// add physics hitbox

};

