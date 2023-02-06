#include "PlayerController.h"

PlayerController::PlayerController(int id)
{
	_playerID = id;
}

PlayerController::~PlayerController()
{

}

json PlayerController::Write()
{
	json me;
	me[JSON_COMPONENT_CLASS] = "PlayerController";
	me[JSON_COMPONENT_CONSTRUCTORS].push_back(_playerID);
	return me;
}

void PlayerController::Start()
{
	int ActivePlayerCount = 0;

	//Create new PlayerInput
	_playerInput = new PlayerInput(_playerID);

	//Get the player's PhysicsBody
	_physicsBody = _gameObject->GetComponent<Physics>();

	//TEMP
	switch (_playerID)
	{
	default:
		//leave unassigned
	case 1:
		_playerInput->SetDeviceType(KeyboardLeft);
		break;
	case 2:
		_playerInput->SetDeviceType(KeyboardRight);
		break;
	}
}

void PlayerController::Update(float deltaTime)
{
	//Movement
	_currentMovement = _playerInput->ReadAxis(Movement);

	//Jump
	if (_playerInput->IsActionDown(Jump))
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

	//Pause
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
	if (_currentMovement.x != 0.0f || _currentMovement.y != 0.0f)
	{
		_physicsBody->ApplyForceToObj(_currentMovement * _moveSpeed, true);
	}

	if (_isJumping)
	{
		Vector2 currentVelocity = _physicsBody->GetLinearVelocity();

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
		else if (currentVelocity.y >= _jumpForce.y)
		{
			_isJumping = false;
		}
		_isJumping = false;
	}
}


void PlayerController::JumpPressed()
{
	if (_jumpReset)
		_isJumping = true;
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