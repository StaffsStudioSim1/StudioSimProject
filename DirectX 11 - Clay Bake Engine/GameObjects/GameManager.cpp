#include "GameManager.h"

GameManager::GameManager()
{
	//stuff for when game starts
	isPaused = false;
	isGravityFlipped = false;
	_currentScene = nullptr;
	_pauseSound = new SoundEffect("Resources/SoundEffects/PauseSound.wav");
}

GameManager::~GameManager()
{
	delete _pauseSound;
}

void GameManager::SceneChanged(Scene* scene)
{
	_currentScene = scene;
	isPaused = false;
}

void GameManager::LevelWin()
{
	//what the game does when win condition is met
	//save level progress
	ifstream file;
	file.open("Resources/SaveFiles/LevelSaves.txt");
	{
		ofstream fw("Resources/SaveFiles/LevelSaves.txt", ofstream::out);
		if (fw.is_open())
		{
			for (int i = 0; i < levelList.size(); i++)
			{
				fw << Levels[i] << "\n";
			}
			fw.close();
		}
	}
	//load next level
	if (SceneManager::GetInstance().GetCurrentSceneID() >= 5)
	{
		SceneManager::GetInstance().LoadScene("Resources/mainmenu.json");
	}
	SceneManager::GetInstance().LoadScene("Resources/" + std::to_string(SceneManager::GetInstance().GetCurrentSceneID() + 1) + ".json");
}

void GameManager::LevelLose()
{
	//not really any lose conditions tho
	//what game does when loss condition is met
	//load lose screen
	SceneManager::GetInstance().LoadScene("Resources/LoseScreen.json");
}

void GameManager::Pause()
{
	if (!ObjectHandler::GetInstance().IsOptionsMenuUIEnabled()) // Prevents opening the pause menu while in the options menu
	{
		//pause the game
		isPaused = !isPaused;
		//set time to 0 - pause time
		//freeze all input
		//play pause sound
		_pauseSound->Play();
		//bring up pause menu
		ObjectHandler::GetInstance().EnablePauseMenuUI(isPaused);
	}
}

void GameManager::GravityFlip()
{
	isGravityFlipped = !isGravityFlipped;
}

bool GameManager::IsGravityFlipped()
{
	return isGravityFlipped;
}

void GameManager::SaveFileCheck()
{
	ifstream file;
	file.open("Resources/SaveFiles/LevelSaves.txt");

	if (file)
	{
		file.close();
		return;
	}
	else 
	{
		CreateSaveFile();
	}
}

void GameManager::CreateSaveFile()
{
	ofstream fw("Resources/SaveFiles/LevelSaves.txt");

	if (fw.is_open())
	{
		for (int i = 0; i < levelList.size(); i++)
		{
			fw << levelList[i] << "\n";
		}
		fw.close();
	}
	else
	{
		//cout << "Problem opening file";
	}
}

