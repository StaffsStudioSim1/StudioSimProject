#include "PlayerMagnetism.h"

void PlayerMagnetism::Start()
{
	m_active = false;
	m_handOffest = new Vector2();
	m_currentHandOffset = m_handOffest;
	m_boxSize = new Vector2();

}

void PlayerMagnetism::Stop()
{
	delete m_currentHandOffset;
	delete m_factingVector;
	delete m_boxSize;
	delete m_handOffest;
}

void PlayerMagnetism::FixedUpdate(float timeStep)
{
	if (m_active)
	{
		//Pysiics Cheack area to find colliders

		//For each collider found]
			//Check if magnetic interface (or class i guess)
			//If found run isBeingMagnetic
	}
}





void PlayerMagnetism::ChangeDirection(PlayerDirection direction)
{
	switch (direction)
	{
	case Left:
		//change facing vector x to  -1
		m_factingVector->x = -1;
		//Current hand ofsett to - of handOffset
		m_currentHandOffset = new Vector2(-m_handOffest->x,-m_handOffest->y);
		break;
	case Right:
		//Change facting vector x to 1
		m_factingVector->x = 1;
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
	m_active = true;
}

void PlayerMagnetism::MagnetOff()
{
	m_active = false;
}


