#include "MagnetBox.h"

void MagnetBox::isBeingMagnetised(Vector2 pushingPlayerPos, MagnetDirection forceDirection, PlayerController* ControlingPlayer)
{
	switch (forceDirection)
	{
	case PullTowards:
		//Force to add pushingPlayerPos - (Vector2)transform.position)*m_pushForce
		m_Physics->ApplyForceToObj(pushingPlayerPos - (Vector2)_gameObject->GetTransform()* m_pushForce);
		break;
	case PushAway:
		//Force to add -(pushingPlayerPos - (Vector2)transform.position)*m_pushForce
		break;
	default:
		break;
	}
}

void MagnetBox::Start()
{
	//TODO: Gett attached physiscs component
	m_Physics = _gameObject->GetComponent<Physics>();
	
}

void MagnetBox::Stop()
{

}