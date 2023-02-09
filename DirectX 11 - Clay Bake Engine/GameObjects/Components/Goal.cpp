#include "Goal.h"

Goal::Goal(std::string levelName)
{
	_NextLevelName = levelName;
}

json Goal::Write()
{
	json me;
	me[JSON_COMPONENT_CLASS] = "Goal";
	me[JSON_COMPONENT_CONSTRUCTORS].push_back(_NextLevelName);
	return me;
}

void Goal::Start()
{
	_Position = _gameObject->GetTransform()->GetPosition();
	_playerCount = 0;
	_GoalSize.x = 1.0f;
	_GoalSize.y = 1.0f;
}

void Goal::Update()
{
	_playerCount = 0; //reset player count
	std::vector<GameObject*> areaCheck = ObjectHandler::GetInstance().GetObjectsInArea(_Position, _GoalSize);

	//check hitbox state
	for (GameObject* object : areaCheck)
	{
		if (object->GetComponent<PlayerController>())
		{
			_playerCount += 1;
		}
	}

	if(_playerCount >= 2)
	{
		//LevelWin();
		GameManager::GetInstance().LevelWin();
	}
	
}


