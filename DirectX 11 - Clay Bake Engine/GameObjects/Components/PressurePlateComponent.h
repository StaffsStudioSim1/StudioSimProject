#pragma once
#include "Interactable.h"

class PressurePlateComponent : public Interactable
{
public:
	bool m_WeighedDown;
	void start();
};

