#include "MovingPlatform.h"
#include <cmath>

void MovingPlatform::SetPlatfromDirection(PlatformDirection newDirection)
{
	if (m_movingDirection != None)
		m_lastDirection = newDirection;
	
	m_movingDirection = newDirection;
}

void MovingPlatform::Update(float timeStep)
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
	Vector2 result;
	result.x =  Lerp(currentPos.x, Target.x, deltaTime);
	result.x = Clamp(result.x, 0, 10);
	result.y = Lerp(currentPos.y, Target.y, deltaTime);
	result.y = Clamp(result.y, 0, 10);


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
