#include "Rigidbody.h"
#include "../GameObject.h"
#include "../GameManager.h"

Rigidbody::Rigidbody()
{
	_mass = 1.0f;
}

void Rigidbody::FixedUpdate(float timeStep)
{
	// Update Net Force
	if (GameManager::GetInstance().IsGravityFlipped())
		_netForce.y += 98.1f;
	else
		_netForce.y -= 98.1f;

	// Update Acceleration
	_acceleration.x = _netForce.x / _mass;
	_acceleration.y = _netForce.y / _mass;

	// Update Position
	Vector2 oldPos = _gameObject->GetTransform()->GetPosition();
	_gameObject->GetTransform()->SetPosition(
		oldPos.x + _velocity.x * timeStep + 0.5f * _acceleration.x * timeStep * timeStep,
		oldPos.y + _velocity.y * timeStep + 0.5f * _acceleration.y * timeStep * timeStep);
	//Vector2 newPos = _gameObject->GetTransform()->GetPosition();
	// Update Velocity
	_velocity.x = _velocity.x + _acceleration.x * timeStep;
	_velocity.y = _velocity.y + _acceleration.y * timeStep;

	_netForce = Vector2();
}

void Rigidbody::AddForce(Vector2 force)
{
	_netForce += force;
}

void Rigidbody::Collide(GameObject* victim)
{
	AABB* myBox = _gameObject->GetComponent<AABB>();
	Rigidbody* victimRB = victim->GetComponent<Rigidbody>();

	if (victimRB == nullptr)
	{
		Vector2 normal = Normalize(victim->GetTransform()->GetPosition() - _gameObject->GetTransform()->GetPosition());
		normal *= -1;
		Vector2 result = _velocity - normal * 1.0f * Dot(normal, _velocity);
		_velocity = result;
	}
	else
	{
		float elasticity = 0.5f;
		Vector2 V1fminusV2f = (_velocity - victimRB->_velocity) * -elasticity;
		Vector2 V1fplusV2f = _velocity + victimRB->_velocity;
		Vector2 v1f = (V1fplusV2f + V1fminusV2f) / 2.0f;
		Vector2 v2f = (V1fplusV2f - V1fminusV2f) / 2.0f;

		_velocity = v1f;
		victimRB->_velocity = v2f;
	}
}

Vector2 Rigidbody::GetVelocity()
{
	return _velocity;
}
