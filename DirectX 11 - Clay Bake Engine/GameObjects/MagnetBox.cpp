#include "MagnetBox.h"

void MagnetBox::isBeingMagnetised(Vector2 pushingPlayerPos, MagnetDirection forceDirection, PlayerController* ControlingPlayer)
{
	switch (forceDirection)
	{
	case Towards:
		//Force to add pushingPlayerPos - (Vector2)transform.position)*m_pushForce
		m_attachedPhysics->ApplyForceToObj();
		break;
	case Away:
		//Force to add -(pushingPlayerPos - (Vector2)transform.position)*m_pushForce
		break;
	default:
		break;
	}
}

void MagnetBox::Start()
{
	//TODO: Gett attached physiscs component
	m_attachedPhysics;
}

void MagnetBox::Stop()
{

}
