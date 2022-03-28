#include "App.h"
#include <sstream>
#include <iomanip>
App::App()
	:wnd(800,600,L"dx11 application")
{

}

int App::Run()
{
	while (true) {
		if (const auto ecode = Window::ProcessMessages()) {
			return *ecode;
		}
		Frame();
	}
	return 0;
}

void App::Frame()
{
	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	wnd.Gfx().ClearBuffer(c, c, 1.0f);
	wnd.Gfx().DrawTestTriangle();

	wnd.Gfx().EndFrame();
}

