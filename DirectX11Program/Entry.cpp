// DirectX11Program.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include <sstream>
#include "src/Window.h"
#define MAX_NAME_STRING 512

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	try {
		WCHAR name[MAX_NAME_STRING];
		LoadString(hInstance, IDS_APPNAME, name, MAX_NAME_STRING);
		Window wnd(800, 300,name );

		MSG msg;
		BOOL result;
		int count = 0;
		// 主消息循环:
		while ((result = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			while (!wnd.mouse.IsEmpty()) {
				auto e = wnd.mouse.Read();
				if (e.GetType() == Mouse::Event::Type::WheelDown) {
					std::wostringstream oss;
					oss << "mouse position:(" << e.GetPosX() << "," << e.GetPosY()<<","<<count++<<")";
					wnd.SetTitle(oss.str());
				}
			}
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