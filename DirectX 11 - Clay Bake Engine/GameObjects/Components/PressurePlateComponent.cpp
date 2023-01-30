#include "PressurePlateComponent.h"

void PressurePlateComponent::start()
{
	m_WeighedDown = false;
}

void PressurePlateComponent::WeighedDown()
{
	m_WeighedDown = true;
}

void PressurePlateComponent::WeightReleased()
{
	m_WeighedDown = false;
}

void PressurePlateComponent::Update()
{
	//call interactable update

}