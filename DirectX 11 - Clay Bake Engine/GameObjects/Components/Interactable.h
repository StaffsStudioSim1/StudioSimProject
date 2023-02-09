#pragma once
#include "../GameObject.h"
#include "Component.h"
#include "Appearance.h"

class Interactable: public Component
{

public:
	enum InteractableLink
	{
		Default = 0,
		SwitchState = 1, //used to open close doors
		SwitchGravity = 2, //used to switch gravity
		Signal3 = 3,	//spare signals
		signal4 = 4
	};

	GameObject* linkedObject;
	void Start();

	void SetLinkedObject(std::string objectName) { _linkedObjectName = objectName; }

	//member variables
	InteractableLink interactableLink; //InteractableLink chooses what the function of each interactable is
	int interactableNum;
	std::string _linkedObjectName; //name of object the signal is sent to

	//void functions overridden by child interactables
	virtual void Interact(); //virtual method for interacting


	void SendSignal(); //send signal function for linked objects to open close etc.

	//constructors
	//Interactable();
	Interactable(InteractableLink switchType, std::string linkedObjectName);
	Interactable(InteractableLink switchType);	
};

