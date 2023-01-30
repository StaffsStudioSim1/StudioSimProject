#include "GameManager.h"

void GameManager::Start()
{
	//stuff for when game starts
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
	//set time to 0 - pause time
}

void GameManager::UnPause()
{
	//close pause menu
	//continue time
}

void GameManager::GravityFlip()
{
	//flip the gravity
}