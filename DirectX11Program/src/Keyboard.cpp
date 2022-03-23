#include "Keyboard.h"

bool Keyboard::KeyIsPressed(unsigned char keycode)
{
	return keyStates[keycode];
}

Keyboard::Event Keyboard::ReadKey()
{
	if (keyBuffer.size() > 0) {
		Keyboard::Event e = keyBuffer.front();
		keyBuffer.pop();
		return e;
	}
	return Keyboard::Event();
}

bool Keyboard::KeyIsEmpty()
{
	return keyBuffer.empty();
}

void Keyboard::FlushKey()
{
	keyBuffer = std::queue<Event>();
}

char Keyboard::ReadChar()
{
	if (charBuffer.size() > 0) {
		unsigned char c = charBuffer.front();
		charBuffer.pop();
		return c;
	}
	return 0;
}

bool Keyboard::CharIsEmpty()
{
	return charBuffer.empty();
}

void Keyboard::FlushChar()
{
	charBuffer = std::queue<char>();
}

void Keyboard::Flush()
{
	FlushKey();
	FlushChar();
}

void Keyboard::EnableAutoRepeat()
{
	autoRepeatEnable = true;
}

void Keyboard::DisableAutoRepeat()
{
	autoRepeatEnable = false;
}

bool Keyboard::AutoRepeatIsEnabled()
{
	return autoRepeatEnable;
}

void Keyboard::OnKeyPressed(unsigned char keycode)
{
	keyStates[keycode] = true;
	keyBuffer.push(Event(Event::Type::Press, keycode));
	TrimBuffer(keyBuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode)
{
	keyStates[keycode] = false;
	keyBuffer.push(Event(Event::Type::Release, keycode));
	TrimBuffer(keyBuffer);
}

void Keyboard::OnChar(char charater)
{
	charBuffer.push(charater);
	TrimBuffer(charBuffer);
}

void Keyboard::ClearState()
{
	keyStates.reset();
}

constexpr unsigned int Keyboard::bufferSize;

constexpr unsigned int Keyboard::nKeys;

