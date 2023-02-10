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
	m_currentHandOffset = m_handOffest;
	m_boxSize =  Vector2();
	//m_magnetDirection = MagnetDirection::PullTowards;
	m_factingVector = Vector2(1, 1);
	m_activeSound = new SoundEffect("Resources/SoundEffects/MagnetBuzzSound.wav",true);
	m_activeSound->SetVolume(0.25f);
	if (m_magnetDirection == PullTowards)
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
		
		std::vector<GameObject*> inFeild = ObjectHandler::GetInstance().GetObjectsInArea(_gameObject->GetTransform()->GetPosition() + m_currentHandOffset, Vector2(40,20));

		 
		for (GameObject* object : inFeild)
		{
			if (object->GetComponent<MagnetismObject>() != nullptr)
			{
				object->GetComponent<MagnetismObject>()->isBeingMagnetised(_gameObject->GetTransform()->GetPosition(), m_magnetDirection, object->GetComponent<PlayerController>());
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
		m_factingVector.x = -1;
		//Current hand ofsett to - of handOffset
		m_currentHandOffset = GetNegVer(m_handOffest);
		break;
	case Right:
		//Change facting vector x to 1
		m_factingVector.x = 1;
		//Current hand offset to handOffset
		m_currentHandOffset = m_handOffest;
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
		m_magnetDirection = MagnetDirection::PushAway;
	else
		m_magnetDirection = MagnetDirection::PullTowards;
}


