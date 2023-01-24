#include "PlayerController.h"

//TODO: Treat this just like the old Unity Input System

void PlayerController::Start()
{
	
}

void PlayerController::Update(float deltaTime)
{
	if (_jumpReset == false)
	{
		_jumpTimer += deltaTime;
		
		if (_jumpTimer > _midAirControlDelay)
		{
			_movementEnabled = true;
		}
		else
		{
			_movementEnabled = false;
		}
	}

	//POLL BUTTON DOWN
}

void PlayerController::FixedUpdate(float timeStep)
{
	if (_movementEnabled)
	{
		//accelerates in direction of pressed input
		//rb.AddForce(m_currentMove * moveSpeed);
		//if (rb.velocity.magnitude > m_topSpeed) //clamps movement to topspeed
		//{
		//	rb.AddForce(-m_currentMove * moveSpeed);
		//}
	}

	if (_isJumping)
	{
		if (!isFlipped)
		{
			//float _currentVelocity = rb.velocity.y;
			//
		}
	}
}

void PlayerController::Stop()
{

}