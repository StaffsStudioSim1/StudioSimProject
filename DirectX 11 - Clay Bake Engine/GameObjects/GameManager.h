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

    GameManager();
public:
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

