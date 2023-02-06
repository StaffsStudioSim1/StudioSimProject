#include "GameManager.h"
#include "../SceneManager.h"
#include "ObjectHandler.h"

void GameManager::Start()
{
	isPaused = false;
	//stuff for when game starts
}

void GameManager::Update()
{
	//update stuff
	if (isPaused = false)
	{
		//pause button - pause the game
		return;
	}
	if (isPaused = true)
	{
		//pause button - play the game
	}
}

void GameManager::FixedUpdate(float timestamp)
{
	//fixed stuff that needs to happen every frame/60fps stuff
}

void GameManager::LevelWin()
{
	//what the game does when win condition is met
	//load win screen
	//save level progress
	SceneManager::GetInstance().LoadScene("Resources/WinScreen.json");
}

void GameManager::LevelLose()
{
	//what game does when loss condition is met
	//load lose screen
}

void GameManager::LevelReset()
{
	//reset the current level
	//restart time
}

void GameManager::Pause()
{
	//pause the game
	isPaused = true;
	//set time to 0 - pause time
	//freeze all input
	//bring up pause menu

}

void GameManager::UnPause()
{
	isPaused = false;
	//close pause menu
	//continue time
}

void GameManager::GravityFlip()
{
	//flip the gravity
}

void GameManager::GravityFlipOn()
{
	isGravityFlipped = true;

	b2Vec2 Gravity = ObjectHandler::GetInstance().GetPhysicsWorld()->world->GetGravity();
	ObjectHandler::GetInstance().GetPhysicsWorld()->world->SetGravity(-Gravity);
}

void GameManager::GravityFlipOff()
{
	isGravityFlipped = false;

	b2Vec2 Gravity = ObjectHandler::GetInstance().GetPhysicsWorld()->world->GetGravity();
	ObjectHandler::GetInstance().GetPhysicsWorld()->world->SetGravity(Gravity);
}

