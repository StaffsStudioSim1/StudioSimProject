#include "GameManager.h"

GameManager::GameManager()
{
	//stuff for when game starts
	_isPaused = false;
	_isGravityFlipped = false;
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
	_isPaused = false;
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
			for (int i = 0; i < _levelList.size(); i++)
			{
				fw << _levels[i] << "\n";
			}
			fw.close();
		}
	}
	//load next level
	if (SceneManager::GetInstance().GetCurrentSceneID() >= 5)
		SceneManager::GetInstance().LoadScene("Resources/mainmenu.json");
	else
		SceneManager::GetInstance().LoadScene("Resources/Level" + std::to_string(SceneManager::GetInstance().GetCurrentSceneID() + 1) + ".json");
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
		_isPaused = !_isPaused;
		//set time to 0 - pause time
		//freeze all input
		//play pause sound
		_pauseSound->Play();
		//bring up pause menu
		ObjectHandler::GetInstance().EnablePauseMenuUI(_isPaused);
	}
}

void GameManager::GravityFlip()
{
	_isGravityFlipped = !_isGravityFlipped;
}

bool GameManager::IsGravityFlipped()
{
	return _isGravityFlipped;
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
		for (int i = 0; i < _levelList.size(); i++)
		{
			fw << _levelList[i] << "\n";
		}
		fw.close();
	}
	else
	{
		//cout << "Problem opening file";
	}
}

