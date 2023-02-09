#pragma once
#include "Component.h"

class DoorComponent : public Component
{
public:
	bool m_Opened;

	void Start();

	json Write();

	void Open();
	void Close();

	void SwitchState();

};