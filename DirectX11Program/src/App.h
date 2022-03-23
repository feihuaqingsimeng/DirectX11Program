#pragma once
#include "platform/win32/Window.h"
#include "common/Timer.h"

class App {
public:
	App();
	int Run();
private:
	void Frame();
private:
	Window wnd;
	Timer timer;
};