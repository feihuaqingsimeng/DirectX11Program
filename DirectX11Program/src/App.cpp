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
	const float t = timer.Peek();
	std::wostringstream oss;
	oss << "time:" << std::setprecision(1) << std::fixed << t << "s";
	//wnd.SetTitle(oss.str());
}

