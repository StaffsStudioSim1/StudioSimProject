#pragma once
//#include "Component.h"
#include "../Scene.h"
#include <iostream>
#include <string>
#include <fstream>
#include "../SceneManager.h"
#include "ObjectHandler.h"
#include "../Input/PlayerInput.h"
#include "../nlohmann/json.hpp"
using namespace std;
using nlohmann::json;

class GameManager
{
    bool _isPaused;
    bool _isGravityFlipped;
    vector <int> _levelList;
    Scene* _currentScene;
    SoundEffect* _pauseSound;
    string _pauseFilePath;
    string _levels[5] = { "Level1", "Level2", "Level3", "Level4", "Level5" };

public:
    GameManager();
    ~GameManager();

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
    void Pause();
    void GravityFlip();
    bool IsGravityFlipped();
    void SaveFileCheck();
    void CreateSaveFile();
};

