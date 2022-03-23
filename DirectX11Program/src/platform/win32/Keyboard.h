#pragma once
#include <queue>
#include <bitset>
class Keyboard {
	friend class Window;
public:
	class Event {
	public:
		enum class Type
		{
			Invalid,
			Press,
			Release
		};
	private:
		Type type;
		unsigned char code;
	public:
		Event()
			:type(Type::Invalid),code(0)
		{}
		Event(Type type,unsigned char code):
			type(type),code(code)
		{}
		bool IsPress() { return type == Type::Press; }
		bool IsRelease() { return type == Type::Release; }
		bool IsValid() { return type != Type::Invalid; }
		unsigned char GetCode()const { return code; }
	};
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator= (const Keyboard&) = delete;
	//key event stuff
	bool KeyIsPressed(unsigned char keycode);
	Event ReadKey();
	bool KeyIsEmpty();
	void FlushKey();
	//char event stuff
	char ReadChar();
	bool CharIsEmpty();
	void FlushChar();
	void Flush();
	//auto repeat control
	void EnableAutoRepeat();
	void DisableAutoRepeat();
	bool AutoRepeatIsEnabled();
private:
	void OnKeyPressed(unsigned char keycode);
	void OnKeyReleased(unsigned char keycode);
	void OnChar(char charater);
	void ClearState();
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer);
private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	bool autoRepeatEnable = false;
	std::bitset<nKeys> keyStates;
	std::queue<Event> keyBuffer;
	std::queue<char> charBuffer;
};

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer)
{
	while (buffer.size() > bufferSize) {
		buffer.pop();
	}
}
