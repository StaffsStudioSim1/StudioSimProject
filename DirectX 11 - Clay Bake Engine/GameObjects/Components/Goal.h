#pragma once
#include "Component.h"
#include "../GameObject.h"
#include "../ObjectHandler.h"
#include "../PlayerController.h"
#include "../../SceneManager.h"

class Goal :
    public Component
{
public:
    int _playerCount;
    Vector2 _goalSize;
    Vector2 _position;
    std::string _nextLevelName;

    Goal(std::string levelName);

    json Write();

    void Start();
    void Update(float deltaTime);

    void LevelWin();

};

