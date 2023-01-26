#include "PlayerController.h"

//Treat this just like the old Unity Input System

void PlayerController::Start()
{
	//Create new PlayerInput
	_playerInput = new PlayerInput();

	//Set player device type
	//TODO: Change depending on what was pressed
	_playerInput->SetDeviceType(KeyboardLeft);
}

void PlayerController::Update(float deltaTime)
{
	//Check for input
	if (_playerInput->IsActionDown(Movement))
	{
		MovePressed();
	}
	else if (_playerInput->IsActionUp(Movement))
	{
		MoveReleased();
	}
	else if (_playerInput->IsActionHeld(Jump))
	{
		JumpPressed();
	}
	else if (_playerInput->IsActionUp(Jump))
	{
		JumpReleased();
	}
	else if (_playerInput->IsActionDown(Interact))
	{
		InteractPressed();
	}
	else if (_playerInput->IsActionDown(Magnet))
	{
		MagnetPressed();
	}
	else if (_playerInput->IsActionDown(Pause))
	{
		PausePressed();
	}

	//Increment jump timer
	if (!_jumpReset)
		_jumpTimer = deltaTime;
	//potential functionality for the midaircontroldelay
}


//Need OnCollisionEnter

void PlayerController::FixedUpdate(float timeStep)
{
	if (_movementEnabled)
	{
		//accelerate in direction of pressed input
		//if (rb.velocity.magnitude < m_topSpeed)
		//{
		//	rb.AddForce(m_currentMove * moveSpeed);
		//}
	}

	if (_isJumping)
	{
		//if flipped
		//float currentVelocity = rb.velocity.y;

		//if (rb.velocity.y < jumpForce.y)
		//{
		//	rb.AddForce(jumpForce, ForceMode2D.Impulse);
		//	isJumping = false;
		//}
	}
}

void PlayerController::MovePressed()
{
	_movementEnabled = true;
}

void PlayerController::MoveReleased()
{
	_movementEnabled = false;
}

void PlayerController::JumpPressed()
{
	if (_jumpReset)
		_isJumping = true;
	//Add velocity 
}

void PlayerController::JumpReleased()
{
	_isJumping = false;
}

void PlayerController::InteractPressed()
{
	//TODO: Link to Tony's interact class
}

void PlayerController::MagnetPressed()
{
	//TODO: Link to Will's magnet class
}

void PlayerController::PausePressed()
{
	//TODO: Link to Ewan's game manager class
}

void PlayerController::Stop()
{
	delete _playerInput;
}