// DirectX11Program.cpp : 定义应用程序的入口点。
//

#include "../framework.h"
#include "App.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	try {
		App app;
		return app.Run();
	}
	catch (const DXException& e) {
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "standard exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}