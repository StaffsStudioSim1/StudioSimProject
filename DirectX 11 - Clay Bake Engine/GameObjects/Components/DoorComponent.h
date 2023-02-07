#pragma once
#include "Interactable.h"

class DoorComponent : public Component
{
public:
	bool m_Opened;
	void start();
	
	void Open();
	void Close();

	DoorComponent();

	void SwitchState();

};