// DirectX11Program.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "src/Window.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	try {
		Window wnd(800, 300, L"dx11 window");

		MSG msg;
		BOOL result;
		// 主消息循环:
		while ((result = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (result == -1) return -1;
		return (int)msg.wParam;
	}
	catch (const DXException& e) {
		MessageBoxA(nullptr, e.what(),e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "standard exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}