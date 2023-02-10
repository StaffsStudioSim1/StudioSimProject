#include "MagnetBox.h"

void MagnetBox::Magnetise(Vector2 pushingPlayerPos, MagnetDirection forceDirection)
{
	switch (forceDirection)
	{
	case PullTowards:
		//Force to add pushingPlayerPos - (Vector2)transform.position)*m_pushForce
		_rigidbody->AddForce((_gameObject->GetTransform()->GetPosition() - pushingPlayerPos) * _pushForce);
		break;
	case PushAway:
		//Force to add -(pushingPlayerPos - (Vector2)transform.position)*m_pushForce
		_rigidbody->AddForce(GetNegVer((_gameObject->GetTransform()->GetPosition() - pushingPlayerPos) *_pushForce));

		break;
	default:
		break;
	}
}

void MagnetBox::Start()
{
	//TODO: Gett attached physiscs component
	_rigidbody = _gameObject->GetComponent<Rigidbody>();
	
}

void MagnetBox::Stop()
{
	
}

json MagnetBox::Write()
{
	json me;
	me[JSON_COMPONENT_CLASS] = "MagnetBox";
	return me;
}
