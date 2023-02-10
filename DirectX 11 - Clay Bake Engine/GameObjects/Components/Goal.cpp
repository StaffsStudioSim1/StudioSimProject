#include "Goal.h"

Goal::Goal(std::string levelName)
{
	_nextLevelName = levelName;
}

json Goal::Write()
{
	json me;
	me[JSON_COMPONENT_CLASS] = "Goal";
	me[JSON_COMPONENT_CONSTRUCTORS].push_back(_nextLevelName);
	return me;
}

void Goal::Start()
{
	_position = _gameObject->GetTransform()->GetPosition();
	_playerCount = 0;
	_goalSize.x = 10.0f;
	_goalSize.y = 10.0f;
}

void Goal::Update(float deltaTime)
{
	_playerCount = 0; //reset player count
	std::vector<GameObject*> areaCheck = ObjectHandler::GetInstance().GetObjectsInArea(_position, _goalSize);

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

void Goal::LevelWin()
{
	SceneManager::GetInstance().LoadScene(_nextLevelName);
}
