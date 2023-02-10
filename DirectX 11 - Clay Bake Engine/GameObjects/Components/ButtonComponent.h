#pragma once
#include "Interactable.h"

class ButtonComponent : public Interactable
{
private:
	float _timer;
	float _timeLimit;
	bool _active;
public:
	void Interact();
	void Update(float deltaTime);
	void Start();

	ButtonComponent(InteractableLink switchType = Default, std::string linkedObjectName = "");
};

