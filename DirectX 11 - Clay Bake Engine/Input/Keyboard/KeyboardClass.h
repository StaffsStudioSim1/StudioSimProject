#pragma once
#include"KeyBoardEvent.h"
#include <queue>

class KeyboardClass
{
private: 
	bool autoRepeatKeys = false; 
	bool autoRepatChars = false;
	bool keyStates[256];
	std::queue<KeyboardEvent> keyBuffer;
	std::queue<unsigned char> charBuffer;
protected:

public:
	KeyboardClass();
	bool IsKeyPressed(const unsigned int scanCode);
	bool IsKeyBufferEmpty();
	bool IsCharBufferEmpty();
	KeyboardEvent ReadKey();
	unsigned char ReadChar();
	void OnKeyPressed(const unsigned int scanCode);
	void OnKeyReleased(const unsigned int scanCode);
	void OnChar(const unsigned char key);
	void EnableAutoRepeatKeys();
	void DisableAutoRepeatKeys();
	void EnableAutoRepeatChars();
	void DisableAutoRepeatChars();
	bool IsKeyAutoRepeat();
	bool IsCharAutoRepeat();

};