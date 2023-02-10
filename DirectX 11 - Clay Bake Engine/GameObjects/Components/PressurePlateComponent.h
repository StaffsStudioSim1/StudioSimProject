#pragma once
#include "Interactable.h"
#include "../ObjectHandler.h"
#include "../PlayerController.h"
#include "../PlayerMagnetism.h"

class PressurePlateComponent : public Interactable
{
public:
	bool weighedDown;
	Vector2 boxSize;
	Vector2 position;
	
	void WeighedDown();
	void WeightReleased();

	json Write();

	void Start();
	void Update(float deltaTime);
	
	PressurePlateComponent(InteractableLink switchType = Default, std::string linkedObjectName = "");
	

};

