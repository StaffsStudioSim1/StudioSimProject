#include "PlayerController.h"


//Treat this just like the old Unity Input System

//Start does not get called

void PlayerController::Start()
{
	int ActivePlayerCount = 0;

	//Create new PlayerInput
	_playerInput = new PlayerInput();

	//Get the player's PhysicsBody
	_physicsBody = _gameObject->GetComponent<Physics>();

	//Set player device type
	//TODO: Change depending on what was pressed
	_playerInput->SetDeviceType(KeyboardLeft);
}

void PlayerController::Update(float deltaTime)
{
	//Movement
	_currentMovement = _playerInput->ReadAxis(Movement);

	if (Magnitude(_currentMovement) != 0)
	{
		MovePressed(_currentMovement);
	}
	else if (_playerInput->IsActionUp(Movement))
	{
		MoveReleased();
	}

	//Jump
	if (_playerInput->IsActionHeld(Jump))
	{
		JumpPressed();
	}
	else if (_playerInput->IsActionUp(Jump))
	{
		JumpReleased();
	}

	//Interact
	if (_playerInput->IsActionDown(Interact))
	{
		InteractPressed();
	}

	//Magnet
	if (_playerInput->IsActionHeld(Magnet))
	{
		MagnetPressed();
	}
	else if (_playerInput->IsActionUp(Magnet))
	{
		MagnetReleased();
	}


	if (_playerInput->IsActionDown(Pause))
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
		if (Magnitude(*_physicsBody->GetLinearVelocity()) < _topSpeed)
		{
			_physicsBody->ApplyForceToObj(_currentMovement, true);
		}
	}

	if (_isJumping)
	{
		Vector2 currentVelocity = *_physicsBody->GetLinearVelocity();

		if (currentVelocity.y < _jumpForce.y)
		{
			if (!isFlipped)
			{
				_physicsBody->ApplyForceToObj(_jumpForce, true);
			}
			else
			{
				_physicsBody->ApplyForceToObj(Vector2(0.0f, -_jumpForce.y), true);
			}

		}

		//if flipped
		//float currentVelocity = rb.velocity.y;

		//if (rb.velocity.y < jumpForce.y)
		//{
		//	rb.AddForce(jumpForce, ForceMode2D.Impulse);
		//	isJumping = false;
		//}
	}
}

void PlayerController::MovePressed(Vector2 direction)
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

void PlayerController::MagnetReleased()
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