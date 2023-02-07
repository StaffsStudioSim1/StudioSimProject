#include "MovingPlatform.h"
#include <cmath>
#include <tchar.h>

MovingPlatform::MovingPlatform(float limit1X, float limit1Y, float limit2X, float limit2Y)
{
	m_limit1 =  Vector2(limit1X, limit1Y);
	m_limit2 =  Vector2(limit2X, limit2Y);

}

void MovingPlatform::SetPlatfromDirection(PlatformDirection newDirection)
{
	if (m_movingDirection != None)
		m_lastDirection = newDirection;
	
	m_movingDirection = newDirection;
}

void MovingPlatform::Update(float deltaTime)
{
	if (m_movingDirection == None)
		return;

	if(m_movingDirection == PlatfromLeft)
	{
		Vector2 toMoveTo = MoveTowards(_gameObject->GetComponent<Physics>()->GetPosition(), m_origin + m_limit1, deltaTime);
		_gameObject->GetComponent<Physics>()->SetLinearVelocity(toMoveTo);
		if (_gameObject->GetTransform()->GetPosition() == m_origin + m_limit1)
		{
			SetPlatfromDirection(PlatfromRight);
		}
	}
	else
	{
		Vector2 toMoveTo = MoveTowards(_gameObject->GetComponent<Physics>()->GetPosition(), m_origin + m_limit2, deltaTime);
		_gameObject->GetComponent<Physics>()->SetLinearVelocity(toMoveTo);
		if (_gameObject->GetTransform()->GetPosition() == m_origin + m_limit2)
		{
			SetPlatfromDirection(PlatfromLeft);
		}
	}

	
}

void MovingPlatform::Start()
{
	
	m_origin = Vector2(_gameObject->GetComponent<Physics>()->GetPosition());
	
}

void MovingPlatform::Stop()
{
	//delete m_origin;

}

Vector2 MovingPlatform::MoveTowards(Vector2 currentPos, Vector2 Target, float deltaTime)
{
	Vector2 result;
	result.x =  Lerp(currentPos.x, Target.x, deltaTime);
	result.x = Clamp(result.x, -10, 10);
	result.y = Lerp(currentPos.y, Target.y, deltaTime);
	result.y = Clamp(result.y, -10, 10);


	return result;
}

float MovingPlatform::Lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

float MovingPlatform::Clamp(float v, float lo, float hi)
{
	if(v < lo)
		return lo;
	if (hi < v)
		return hi;
	return v;

}
