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
	wcex.hIcon = nullptr;
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
}

Window::~Window()
{
	DestroyWindow(hwnd);
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
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

Window::Exception::Exception(int line, const char* file, HRESULT hr)
	:DXException(line,file),hr(hr)
{

}

const char* Window::Exception::what() const
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code]" << GetErrorCode() << std::endl
		<< "[Description]" << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::Exception::GetType() const
{
	return "WindowException";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr)
{
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&pMsgBuf),
		0, nullptr);
	if (nMsgLen == 0)
		return "Unidentified error code";
	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const
{
	return hr;
}

std::string Window::Exception::GetErrorString() const
{
	return TranslateErrorCode(hr);
}
