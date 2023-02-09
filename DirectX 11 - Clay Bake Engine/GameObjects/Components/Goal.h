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
    Vector2 _GoalSize;
    Vector2 _Position;
    std::string _NextLevelName;

    void Start();
    
    void Update();

    void LevelWin();

    Goal(std::string);
};

