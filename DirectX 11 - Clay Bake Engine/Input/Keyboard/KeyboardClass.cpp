#include "KeyboardClass.h"
#include <Windows.h>

KeyboardClass::KeyboardClass()
{
	for (int i = 0; i < 256; i++)
		this->_keyStates[i] = false;
}

bool KeyboardClass::IsKeyPressed(const unsigned int scanCode)
{
	if (scanCode < 0)
		return false;
	return this->_keyStates[scanCode];
}

bool KeyboardClass::IsKeyBufferEmpty()
{
	return this->keyBuffer.empty();
}

bool KeyboardClass::IsCharBufferEmpty()
{
	return this->charBuffer.empty();
}

KeyboardEvent KeyboardClass::ReadKey()
{
	if (this->keyBuffer.empty()) // if no key is read
	{
		return KeyboardEvent(); // return empty keyboard event
	}
	else
	{
		KeyboardEvent j = this->keyBuffer.front(); //gets first item in 
		this->keyBuffer.pop(); // remove first item from queue
		return j;	// return event
	}
}

unsigned char KeyboardClass::ReadChar()
{
	if (this->charBuffer.empty())
	{
		return 0u; // return null char
	}
	else
	{
		unsigned char j = this->charBuffer.front(); // get first char
		this->charBuffer.pop(); // remove char form queue
		return j;
	}
}

void KeyboardClass::OnKeyPressed(const unsigned int scanCode)
{
	this->_keyStates[scanCode] = true;
	//this->keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, key));
}

void KeyboardClass::OnKeyReleased(const unsigned int scanCode)
{
	this->_keyStates[scanCode] = false;
	//this->keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, key));
}

void KeyboardClass::OnChar(const unsigned char key)
{
	this->charBuffer.push(key);
}

void KeyboardClass::EnableAutoRepeatKeys()
{
	this->_autoRepeatKeys = true;
}

void KeyboardClass::DisableAutoRepeatKeys()
{
	this->_autoRepeatKeys = false;
}

void KeyboardClass::EnableAutoRepeatChars()
{
	this->_autoRepatChars = true;
}

void KeyboardClass::DisableAutoRepeatChars()
{
	this->_autoRepatChars = false;
}

bool KeyboardClass::IsKeyAutoRepeat()
{
	return this->_autoRepeatKeys;
}

bool KeyboardClass::IsCharAutoRepeat()
{
	return this->_autoRepatChars;
}