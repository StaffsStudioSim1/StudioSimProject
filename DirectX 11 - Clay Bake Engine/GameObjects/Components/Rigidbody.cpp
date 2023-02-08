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
	AABB* victimBox = victim->GetComponent<AABB>();
	Rigidbody* victimRB = victim->GetComponent<Rigidbody>();

	if (victimRB == nullptr)
	{
		// Pen depth based on https://gamedev.stackexchange.com/questions/171725/2d-aabb-collision
		std::vector<Vector2> axes;
		axes.push_back(Vector2(0, 1));
		axes.push_back(Vector2(0, -1));
		axes.push_back(Vector2(1, 0));
		axes.push_back(Vector2(-1, 0));

		std::vector<Vector2> pointsA;
		pointsA.push_back(_gameObject->GetTransform()->GetPosition() + Vector2(myBox->GetSize().x / 2, myBox->GetSize().y / 2));
		pointsA.push_back(_gameObject->GetTransform()->GetPosition() + Vector2(-myBox->GetSize().x / 2, myBox->GetSize().y / 2));
		pointsA.push_back(_gameObject->GetTransform()->GetPosition() + Vector2(myBox->GetSize().x / 2, -myBox->GetSize().y / 2));
		pointsA.push_back(_gameObject->GetTransform()->GetPosition() + Vector2(-myBox->GetSize().x / 2, -myBox->GetSize().y / 2));

		std::vector<Vector2> pointsB;
		pointsB.push_back(victim->GetTransform()->GetPosition() + Vector2(victimBox->GetSize().x / 2, victimBox->GetSize().y / 2));
		pointsB.push_back(victim->GetTransform()->GetPosition() + Vector2(-victimBox->GetSize().x / 2, victimBox->GetSize().y / 2));
		pointsB.push_back(victim->GetTransform()->GetPosition() + Vector2(victimBox->GetSize().x / 2, -victimBox->GetSize().y / 2));
		pointsB.push_back(victim->GetTransform()->GetPosition() + Vector2(-victimBox->GetSize().x / 2, -victimBox->GetSize().y / 2));

		float min_penetration = INT_MAX;

		Vector2 normal;

		for (Vector2 axis : axes)
		{
			float minA = INT_MAX;
			float maxA = INT_MIN;
			float minB = INT_MAX;
			float maxB = INT_MIN;

			for (Vector2 point : pointsA)
			{
				float dot = Dot(point, axis);
				if (maxA < dot)
					maxA = dot;
				if (minA > dot)
					minA = dot;
			}

			for (Vector2 point : pointsB)
			{
				float dot = Dot(point, axis);
				if (maxB < dot)
					maxB = dot;
				if (minB > dot)
					minB = dot;
			}

			float penetration = maxA - minB;

			if (penetration < min_penetration && penetration >= 0)
			{
				min_penetration = penetration;
				normal = axis;
			}
		}

		Vector2 result = _velocity - normal * Dot(normal, _velocity);
		_velocity = result;

		//float penetrationMultiplier = 1 + min_penetration * 10.0;
		//AddForce(normal * penetrationMultiplier);
		_gameObject->GetTransform()->SetPosition(_gameObject->GetTransform()->GetPosition() - normal * min_penetration);
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
