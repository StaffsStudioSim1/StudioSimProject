#pragma once
#include <string>
#include "Scene.h"
class SceneManager
{
public:
	// Singleton Code
	static SceneManager& GetInstance()
	{
		static SceneManager instance;
		return instance;
	}

	SceneManager(SceneManager const&) = delete;
	void operator=(SceneManager const&) = delete;

	void Initialise(int width, int height);

	void LoadScene(std::string filePath);
	bool ShouldSceneChange();
	Scene* ReadScene();
	void SetCurrentSceneID(int id) { _currentSceneID = id; }
	int GetCurrentSceneID();
	std::string GetCurrentSceneFilePath();
private:
	SceneManager();
	~SceneManager();

	bool _shouldSceneChange;
	Scene* _newScene;
	int _currentSceneID;
	std::string _currentSceneFilePath;

	int _width;
	int _height;
};

