#pragma once
#include <queue>
class Mouse {
	friend class Window;
public:
	class Event {
	public:
		enum class Type {
			Invalid,
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Leave,
			Enter,
		};
	private:
		Type type;
		bool leftIsPressed;
		bool rightIsPressed;
		int x;
		int y;
	public :
		Event()
			:type(Type::Invalid), leftIsPressed(false), rightIsPressed(false), x(0), y(0) 
		{
		}
		Event(Type type, const Mouse& parent)
			:type(type), leftIsPressed(parent.leftIsPressed), rightIsPressed(parent.rightIsPressed), x(parent.x), y(parent.y)
		{}
		bool IsValid() {
			return type != Type::Invalid;
		}
		Type GetType() { return type; }
		std::pair<int, int> GetPos() const {
			return { x,y };
		}
		int GetPosX() const { return x; }
		int GetPosY() const { return y; }
		bool LeftIsPress() const { return leftIsPressed; }
		bool RightIsPress() const { return rightIsPressed; }

	};
public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	std::pair<int, int> GetPos() const;
	int GetPosX() const;
	int GetPosY() const;
	bool LeftIsPress() const;
	bool RightIsPress() const;
	Mouse::Event Read();
	bool IsEmpty() const { return buffer.empty(); }
	void Flush();
	bool IsInWindow();
	void OnMouseLeave();
	void OnMouseEnter();
private:
	void OnMouseMove(int x, int y);
	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void OnWheelDelta(int x, int y,int delta);
	void TrimBuffer();
private:
	static constexpr unsigned int bufferSize = 16u;
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	int x;
	int y;
	int wheelDelta = 0;
	bool isInWindow = false;
	std::queue<Event> buffer;
};