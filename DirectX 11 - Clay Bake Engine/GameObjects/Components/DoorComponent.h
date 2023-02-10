#pragma once
#include "Component.h"

class DoorComponent : public Component
{
private:
	bool _defaultState;
	bool _open;
public:
	void Start();
	json Write();

	void Open();
	void Close();
	void SwitchState();

	DoorComponent(bool defaultState);
};