#include "GameManager.h"
#include "../SceneManager.h"
#include "ObjectHandler.h"
#include "../Input/PlayerInput.h"


void GameManager::Start()
{
	//stuff for when game starts
	isPaused = false;
}

void GameManager::Update()
{
	//update stuff
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
	if (isPaused)
		return;

	isPaused = true;
	//set time to 0 - pause time
	//freeze all input
	//bring up pause menu

}

void GameManager::UnPause()
{
	if (!isPaused)
		return;
	isPaused = false;
	//close pause menu
	//continue time
}

void GameManager::GravityFlip()
{
	//flip the gravity
	if(isGravityFlipped)
		GravityFlipOff();
	else
		GravityFlipOn();
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

