#include "MagnetBox.h"

void MagnetBox::isBeingMagnetised(Vector2 pushingPlayerPos, MagnetDirection forceDirection, PlayerController* ControlingPlayer)
{
	switch (forceDirection)
	{
	case PullTowards:
		//Force to add pushingPlayerPos - (Vector2)transform.position)*m_pushForce
		m_rigidbody->AddForce(pushingPlayerPos - _gameObject->GetTransform()->GetPosition() * m_PushForce);
		break;
	case PushAway:
		//Force to add -(pushingPlayerPos - (Vector2)transform.position)*m_pushForce
		m_rigidbody->AddForce(GetNegVer(pushingPlayerPos - _gameObject->GetTransform()->GetPosition() *m_PushForce));

		break;
	default:
		break;
	}
}

void MagnetBox::Start()
{
	//TODO: Gett attached physiscs component
	m_rigidbody = _gameObject->GetComponent<Rigidbody>();
	
}

void MagnetBox::Stop()
{
	
}
