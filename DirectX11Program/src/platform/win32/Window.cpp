#include "Window.h"
#include <sstream>
Window::WindowClass Window::WindowClass::wndClass;

constexpr const wchar_t* Window::WindowClass::wndClassName;

Window::WindowClass::WindowClass() noexcept
	:hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_OWNDC;
	wcex.lpfnWndProc = HandleMsgSetUp;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = LoadIcon(hInst,MAKEINTRESOURCE(IDI_APPICON));
	wcex.hCursor = nullptr;
	wcex.hbrBackground = nullptr;
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = GetName();
	wcex.hIconSm = nullptr;
	 RegisterClassExW(&wcex);
}
Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, GetInstance());
}



const wchar_t * Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}


HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}



Window::Window(int width, int height, const wchar_t* name)
	:width(width),height(height)
{
	
	RECT r;
	r.left = 100;
	r.right = width + r.left;
	r.top = 100;
	r.bottom = r.top + height;

	AdjustWindowRect(&r, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	hwnd = CreateWindow(WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, r.right-r.left, r.bottom-r.top, nullptr, nullptr,
		WindowClass::GetInstance(), this);

	ShowWindow(hwnd, SW_SHOWDEFAULT);

	graphics = std::make_unique<Graphics>(hwnd);
}

Window::~Window()
{
	DestroyWindow(hwnd);
}

void Window::SetTitle(const std::wstring& name)
{
	SetWindowText(hwnd, name.c_str());
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg = {0};
	PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
	if (msg.message == WM_QUIT)
		return msg.wParam;

	TranslateMessage(&msg);
	DispatchMessage(&msg);
	return {};
}

Graphics & Window::Gfx()
{
	return *graphics;
}

LRESULT Window::HandleMsgSetUp(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE) {
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pwnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pwnd));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		return pwnd->HandleMsg(hwnd, msg, wParam, lParam);
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* const pwnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd,GWLP_USERDATA));
	return pwnd->HandleMsg(hwnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_KILLFOCUS:
		keyboard.ClearState();
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		//第三十位标记位为首次
		if(!(lParam & 0x40000000)|| keyboard.AutoRepeatIsEnabled())
			keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		keyboard.OnChar(static_cast<unsigned char>(wParam));
		break;
	/**********************mouse************************/
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height) 
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow()) {
				SetCapture(hwnd);
				mouse.OnMouseEnter();
			}
		}
		else {
			if (wParam &(MK_LBUTTON | MK_RBUTTON)) {
				mouse.OnMouseMove(pt.x, pt.y);
			}
			else {
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		
		break;
	}
		
	case WM_LBUTTONDOWN:
	{
		const POINTS p = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(p.x, p.y);
		break;
	}
		
	case WM_LBUTTONUP:
	{
		const POINTS p = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(p.x, p.y);
		break;
	}
		
	case WM_RBUTTONDOWN:
	{
		const POINTS p = MAKEPOINTS(lParam);
		mouse.OnRightPressed(p.x, p.y);
		break;
	}
		
	case WM_RBUTTONUP:
	{
		const POINTS p = MAKEPOINTS(lParam);
		mouse.OnRightReleased(p.x, p.y);
	}
		
	case WM_MOUSEWHEEL:
	{
		const POINTS p = MAKEPOINTS(lParam);
		int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(p.x, p.y, delta);
	}
		
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

