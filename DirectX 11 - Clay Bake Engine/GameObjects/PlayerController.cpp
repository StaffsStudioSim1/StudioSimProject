#include "PlayerController.h"
#include "PlayerMagnetism.h"

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
	//Create new PlayerInput
	_playerInput = new PlayerInput(_playerID);

	//Get the player's RigidBody
	_rigidbody = _gameObject->GetComponent<Rigidbody>();

	//Init SoundEffects
	_jumpSoundEffect = new SoundEffect("Resources/Laser_Shoot3.wav");
	_jumpSoundEffect->SetVolume(0.25f);
	_moveSoundEffect = new SoundEffect("Resources/SoundEffects/MetalWalkNoise.wav", true);
	_moveSoundEffect->SetVolume(0.25f);
	_magnet = _gameObject->GetComponent<PlayerMagnetism>();
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
	if (_activeJumpTimer >= _jumpTimer)
	{
		_jumpReset = true;
		_activeJumpTimer = 0.0f;
	}
	else
	{
		_activeJumpTimer += deltaTime;
	}
}

void PlayerController::FixedUpdate(float timeStep)
{
	_rigidbody->SetInput(_currentMovement * _moveSpeed);

	if (_currentMovement.x != 0.0f || _currentMovement.y != 0.0f)
	{
		if (!_isWalking)
		{
			_moveSoundEffect->Play();
		}
		_isWalking = true;
	}
	else
	{
		_isWalking = false;
		_moveSoundEffect->Stop();
	}

	if (_currentMovement.x < 0.0f)
	{
		_facingDirection = Left;
		_magnet->ChangeDirection(_facingDirection);
		//Magnet code
	}
	else if (_currentMovement.x > 0.0f)
	{
		_facingDirection = Right;
		_magnet->ChangeDirection(_facingDirection);
		//Magnet code
	}
}


void PlayerController::JumpPressed()
{
	if (!_jumpReset)
		return;

	_jumpReset = false;

	if (!GameManager::GetInstance().IsGravityFlipped())
	{
		_rigidbody->AddForce(_jumpForce);
	}
	else
	{
		_rigidbody->AddForce(-_jumpForce);
	}

	_jumpSoundEffect->Play();
}

void PlayerController::InteractPressed()
{
	//TODO: Link to Tony's interact class
}

void PlayerController::MagnetPressed()
{
	_magnet->MagnetOn();
}

void PlayerController::MagnetReleased()
{
	_magnet->MagnetOff();
}

void PlayerController::PausePressed()
{
	//TODO: Link to Ewan's game manager class
}

void PlayerController::Stop()
{
	delete _playerInput;
	delete _jumpSoundEffect;
	delete _moveSoundEffect;
}