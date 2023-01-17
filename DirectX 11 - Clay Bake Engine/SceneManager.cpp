#include "SceneManager.h"

SceneManager::SceneManager()
{
	_shouldSceneChange = false;
	_newScene = nullptr;
}

SceneManager::~SceneManager()
{
}

void SceneManager::LoadScene(std::string filePath)
{
	if (_shouldSceneChange)
		delete _newScene;
	_shouldSceneChange = true;
	_newScene = new Scene(filePath);

}

bool SceneManager::ShouldSceneChange()
{
	return _shouldSceneChange;
}

Scene* SceneManager::ReadScene()
{
	_shouldSceneChange = false;
	return _newScene;
}
