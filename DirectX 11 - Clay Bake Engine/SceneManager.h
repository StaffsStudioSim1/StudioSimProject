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
	int GetCurrentSceneID();
private:
	SceneManager();
	~SceneManager();

	bool _shouldSceneChange;
	Scene* _newScene;
	int _currentSceneID;

	int _width;
	int _height;
};

