#pragma once
#include "Interactable.h"
class DoorComponent :
    public Interactable
{
};

#pragma once
#include "Interactable.h"

class DoorComponent : public Interactable
{
public:
	bool m_Opened;
	void start();
	void Interact();


	void Open();
	void Close();

	void SwitchState()

};