#include "PlayerMagnetism.h"
#include "ObjectHandler.h"


json PlayerMagnetism::Write()
{
	json me;
	me[JSON_COMPONENT_CLASS] = "PlayerMagnetism";
	return me;
}

void PlayerMagnetism::Start()
{
	m_magnetActive = false;
	m_handOffest =  Vector2(10,0);
	_currentHandOffset = m_handOffest;
	m_boxSize =  Vector2();
	//m_magnetDirection = MagnetDirection::PullTowards;
	_facingVector = Vector2(1, 1);
	m_activeSound = new SoundEffect("Resources/SoundEffects/MagnetBuzzSound.wav",true);
	m_activeSound->SetVolume(0.25f);
	if (_magnetDirection == PullTowards)
	{
		m_activeSound->SetPitch(0.5f);
	}
}

void PlayerMagnetism::Stop()
{
	delete m_activeSound;
}

void PlayerMagnetism::FixedUpdate(float timeStep)
{
	if (m_magnetActive)
	{
		
		std::vector<GameObject*> inFeild = ObjectHandler::GetInstance().GetObjectsInArea(_gameObject->GetTransform()->GetPosition() + _currentHandOffset, Vector2(40,20));

		 
		for (GameObject* object : inFeild)
		{
			if (object->GetComponent<MagnetismObject>() != nullptr)
			{
				object->GetComponent<MagnetismObject>()->Magnetise(_gameObject->GetTransform()->GetPosition(), _magnetDirection);
			}
		}

		//Pysiics Cheack area to find colliders
		//GameObject* foundObjects[] = 
		//For each collider found]
			//Check if magnetic interface (or class i guess)
			//If found run isBeingMagnetic
	}
}

void PlayerMagnetism::ChangeDirection(FacingDirection direction)
{
	switch (direction)
	{
	case Left:
		//change facing vector x to  -1
		_facingVector.x = -1;
		//Current hand ofsett to - of handOffset
		_currentHandOffset = GetNegVer(m_handOffest);
		break;
	case Right:
		//Change facting vector x to 1
		_facingVector.x = 1;
		//Current hand offset to handOffset
		_currentHandOffset = m_handOffest;
		break;
	default:
		//Change direction faled.
		break;
	}
		
}

void PlayerMagnetism::MagnetOn()
{
	m_magnetActive = true;
	m_activeSound->Play();
}

void PlayerMagnetism::MagnetOff()
{
	m_magnetActive = false;
	m_activeSound->Stop();
}

void PlayerMagnetism::SetMagnetPushPull(int playerID)
{
	if (playerID == 1)
		_magnetDirection = MagnetDirection::PushAway;
	else
		_magnetDirection = MagnetDirection::PullTowards;
}


