#pragma once
#include "Interactable.h"

class DoorComponent : public GameObject
{
public:
	bool m_Opened;
	void start();
	
	void Open();
	void Close();

	void SwitchState();

};