#include"KeyboardClass.h"

KeyboardClass::KeyboardClass()
{
	for (int i = 0; i < 256; i++)
	{
		this->keyStates[i] = false;
	}
}

bool KeyboardClass::KeyIsPressed(const unsigned char keycode)
{
	return this->keyStates[keycode];
}

bool KeyboardClass::KeyBufferIsEmpty()
{
	return this->keyBuffer.empty();
}

bool KeyboardClass::CharBufferIsEmpty()
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

void KeyboardClass::OnKeyPressed(const unsigned char key)
{
	this->keyStates[key] = true;
	this->keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, key));
}

void KeyboardClass::OnKeyReleased(const unsigned char key)
{
	this->keyStates[key] = true;
	this->keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, key));
}

void KeyboardClass::OnChar(const unsigned char key)
{
	this->charBuffer.push(key);
}

void KeyboardClass::EnableAutoRepeatKeys()
{
	this->autoRepeatKeys = true;
}

void KeyboardClass::DisableAutoRepeatKeys()
{
	this->autoRepeatKeys = false;
}

void KeyboardClass::EnableAutoRepeatChars()
{
	this->autoRepatChars = true;
}

void KeyboardClass::DisableAutoRepeatChars()
{
	this->autoRepatChars = false;
}

bool KeyboardClass::IsKeyAutoRepeat()
{
	return this->autoRepeatKeys;
}

bool KeyboardClass::IsCharAutoRepeat()
{
	return this->autoRepatChars;
}