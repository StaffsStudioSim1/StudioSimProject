#include "PlayerMagnetism.h"
#include "ObjectHandler.h"

void PlayerMagnetism::Start()
{
	m_magnetActive = false;
	m_handOffest =  Vector2();
	m_currentHandOffset = m_handOffest;
	m_boxSize =  Vector2();

}

void PlayerMagnetism::Stop()
{
	
}

void PlayerMagnetism::FixedUpdate(float timeStep)
{
	if (m_magnetActive)
	{
		
		std::vector<GameObject*> inFeild = ObjectHandler::GetInstance().GetObjectsInArea(_gameObject->GetTransform()->GetPosition() + m_currentHandOffset, Vector2(20,20));

		 if(inFeild.size() == 0)
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
		m_currentHandOffset = Vector2(-m_handOffest.x,-m_handOffest.y);
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
}

void PlayerMagnetism::MagnetOff()
{
	m_magnetActive = false;
}


