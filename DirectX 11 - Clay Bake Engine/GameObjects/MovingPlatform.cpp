#include "MovingPlatform.h"
#include <cmath>

void MovingPlatform::SetPlatfromDirection(PlatformDirection newDirection)
{
	if (m_movingDirection != None)
		m_lastDirection = newDirection;
	
	m_movingDirection = newDirection;
}

void MovingPlatform::FixedUpdate(float timeStep)
{
	if (m_movingDirection == None)
		return;

	if(m_movingDirection == PlatfromLeft)
	{
		m_Physics->SetTransform(MoveTowards(m_Physics->GetPosition(), *m_oragin + *m_limit1, timeStep), m_Physics->GetAngleDegress());
		if (m_Physics->GetPosition() == *m_oragin + *m_limit1)
		{
			SetPlatfromDirection(PlatfromRight);
		}
	}
	else
	{
		m_Physics->SetTransform(MoveTowards(m_Physics->GetPosition(), *m_oragin + *m_limit2, timeStep), m_Physics->GetAngleDegress());
		if (m_Physics->GetPosition() == *m_oragin + *m_limit2)
		{
			SetPlatfromDirection(PlatfromLeft);
		}
	}
}

void MovingPlatform::Start()
{
	m_Physics = _gameObject->GetComponent<Physics>();
	m_oragin = new Vector2(m_Physics->GetPosition());
	m_limit1 = new Vector2(-10.0f, 0.0f);
	m_limit2 = new Vector2(10.0f, 0.0f);
}

void MovingPlatform::Stop()
{
	delete m_oragin;
	delete m_limit1;
	delete m_limit2;
}

Vector2 MovingPlatform::MoveTowards(Vector2 currentPos, Vector2 Target, float deltaTime)
{
	Vector2 lerpResult;
	lerpResult.x =  std::lerp(currentPos.x, Target.x, deltaTime);
	return Vector2();
}
