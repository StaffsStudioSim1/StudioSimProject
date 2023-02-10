#include "KeyBoardEvent.h"

KeyboardEvent::KeyboardEvent() : _type(EventType::Invalid), _key(0u)
{

}

KeyboardEvent::KeyboardEvent(const EventType type, const unsigned char key) : _type(type), _key(key)
{

}

bool KeyboardEvent::IsPress() const
{
	return this->_type == EventType::Press;
}

bool KeyboardEvent::IsRelease() const
{
	return this->_type == EventType::Release;
}

bool KeyboardEvent::IsValid() const
{
	return this->_type != EventType::Invalid;
}

unsigned char KeyboardEvent::GetKeyCode() const
{
	return this->_key;
}