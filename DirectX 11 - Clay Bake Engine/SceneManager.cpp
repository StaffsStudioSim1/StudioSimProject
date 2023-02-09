#include "SceneManager.h"

SceneManager::SceneManager()
{
	_shouldSceneChange = false;
	_newScene = nullptr;
	_currentSceneID = -1;
}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialise(int width, int height)
{
	_width = width;
	_height = height;
}

void SceneManager::LoadScene(std::string filePath)
{
	if (_shouldSceneChange)
		delete _newScene;
	_shouldSceneChange = true;

	_newScene = new Scene(filePath, _width, _height);
	_currentSceneFilePath = filePath;
}

bool SceneManager::ShouldSceneChange()
{
	return _shouldSceneChange;
}

Scene* SceneManager::ReadScene()
{
	_shouldSceneChange = false;
	_currentSceneID = _newScene->GetID();
	return _newScene;
}

int SceneManager::GetCurrentSceneID()
{
	return _currentSceneID;
}

std::string SceneManager::GetCurrentSceneFilePath()
{
	return _currentSceneFilePath;
}
