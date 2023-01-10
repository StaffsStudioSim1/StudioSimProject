#pragma once
#include <map>

enum Action { Move, Jump, Interact, Magnet, Pause };

class ActionMap
{
public:

private:
	std::map<Action, bool> _actionDown;
	std::map<Action, bool> _actionHeld;
};

