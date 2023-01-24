#include "PlayerMagnetism.h"

void PlayerMagnetism::Start()
{
	
}

void PlayerMagnetism::Stop()
{

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
		//Current hand ofsett to - of handOffset
		break;
	case Right:
		//Change facting vector x to 1
		//Current hand offset to handOffset
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


