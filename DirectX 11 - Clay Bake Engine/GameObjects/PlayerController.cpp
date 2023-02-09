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

	// Get the player's Transformation Info
	_playerTransform = _gameObject->GetTransform();

	//Get the player's Appearance
	_playerAppearance = _gameObject->GetComponent<Appearance>();

	//Get the player's Magnet Component
	_magnet = _gameObject->GetComponent<PlayerMagnetism>();
	_magnet->SetMagnetPushPull(_playerID);

	//Init SoundEffects
	_jumpSoundEffect = new SoundEffect("Resources/Laser_Shoot3.wav");
	_jumpSoundEffect->SetVolume(0.25f);
	_moveSoundEffect = new SoundEffect("Resources/SoundEffects/MetalWalkNoise.wav", true);
	_moveSoundEffect->SetVolume(0.25f);
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
		_magnet->MagnetOn();
		_playerState = MagnetState;
	}
	else if (_playerInput->IsActionUp(Magnet))
	{
		_magnet->MagnetOff();
	}

	//Pause
	if (_playerInput->IsActionDown(Pause))
	{
		GameManager::GetInstance().Pause();
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

	//Increment animation timer
	if (_activeFrameDelay >= _animationFrameDelay)
	{
		_currentFrame++;
		_activeFrameDelay = 0.0f;
	}
	else
	{
		_activeFrameDelay += deltaTime;
	}

	switch (_playerState)
	{
	case IdleState:
	{
		_currentFrame = _currentFrame % 4;
		_playerAppearance->SetTexPos(_currentFrame, 1.0f);
	}
	break;
	case WalkState:
	{
		_currentFrame = _currentFrame % 6;
		_playerAppearance->SetTexPos(_currentFrame, 0.0f);
	}
	break;
	case JumpState:
	{
		_currentFrame = _currentFrame % 1;
		_playerAppearance->SetTexPos(0.0f, 2.0f);
	}
	break;
	case FallState:
	{
		_currentFrame = _currentFrame % 1;
		_playerAppearance->SetTexPos(0.0f, 3.0f);
	}
	break;
	case MagnetState:
	{
		_currentFrame = _currentFrame % 4;
		_playerAppearance->SetTexPos(_currentFrame, 5.0f);
	}
	break;
	}
}

void PlayerController::FixedUpdate(float timeStep)
{
	_rigidbody->SetInput(_currentMovement * _moveSpeed);

	if (_currentMovement.x != 0.0f || _currentMovement.y != 0.0f)
	{
		_isWalking = true;
		_playerState = WalkState;
	}
	else
	{
		_isWalking = false;
		_moveSoundEffect->Stop();
		_playerState = IdleState;
	}

	if (_rigidbody->GetVelocity().y < 0.0f)
	{
		_playerState = FallState;
		_moveSoundEffect->Stop();
	}
	else if (_rigidbody->GetVelocity().y > 0.0f)
	{
		_playerState = JumpState;
		_moveSoundEffect->Stop();
	}
	else if (_isWalking)
	{
		_moveSoundEffect->Play();
	}

	if (_currentMovement.x < 0.0f)
	{
		if (_facingDirection == Right)
		{
			_playerTransform->FlipHorizontal(true);
		}
		_facingDirection = Left;
		_magnet->ChangeDirection(_facingDirection);
	}
	else if (_currentMovement.x > 0.0f)
	{
		if (_facingDirection == Left)
		{
			_playerTransform->FlipHorizontal(false);
		}
		_facingDirection = Right;
		_magnet->ChangeDirection(_facingDirection);
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
	// player position
	Vector2 playerPosition = _gameObject->GetTransform()->GetPosition();

	//check area
	std::vector<GameObject*> areaCheck = ObjectHandler::GetInstance().GetObjectsInArea(playerPosition, _interactArea);

	// call interact if component is not null
	for (GameObject* object : areaCheck)
	{
		if (object->GetComponent<Interactable>() != nullptr)
		{
			object->GetComponent<Interactable>()->Interact();
		}
	}
}

void PlayerController::Stop()
{
	delete _playerInput;
	delete _jumpSoundEffect;
	delete _moveSoundEffect;
}