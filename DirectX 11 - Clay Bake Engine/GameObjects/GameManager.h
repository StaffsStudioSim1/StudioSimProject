#pragma once
//#include "Component.h"
#include "../Scene.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class GameManager
{
    bool isPaused;
    bool isGravityFlipped;
    vector <int> levelList;
    Scene* _currentScene;
    string Levels[5] = { "Level1", "Level2", "Level3", "Level4", "Level5" };

public:
    GameManager();
    // Singleton Code
    static GameManager& GetInstance()
    {
        static GameManager instance;
        return instance;
    }

    GameManager(GameManager const&) = delete;
    void operator=(GameManager const&) = delete;

    void SceneChanged(Scene* scene);
    void LevelWin();
    void LevelLose();
    void LevelReset();
    void Pause();
    void UnPause();
    void GravityFlip();
    bool IsGravityFlipped();
    void SaveFileCheck();
    void CreateSaveFile();
};

