#pragma once
#include "Component.h"

class DoorComponent : public Component
{
private:
	bool _open;
public:
	bool _defaultState;

	void Start();
	json Write();

	void Open();
	void Close();
	void SwitchState();

	DoorComponent(bool defaultState);
};