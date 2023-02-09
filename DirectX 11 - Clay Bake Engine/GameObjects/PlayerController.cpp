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
	//create new GameManager
	_gameManager = new GameManager();
	//Create new PlayerInput
	_playerInput = new PlayerInput(_playerID);
	
	//Get the player's RigidBody
	_rigidbody = _gameObject->GetComponent<Rigidbody>();

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
		//Magnet code
	}
	else if (_currentMovement.x > 0.0f)
	{
		_facingDirection = Right;
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
	//GameManager::Pause;
	//GameManager::UnPause;
	_gameManager->Pause();
	_gameManager->UnPause();
}

void PlayerController::Stop()
{
	delete _playerInput;
	delete _jumpSoundEffect;
	delete _moveSoundEffect;
}