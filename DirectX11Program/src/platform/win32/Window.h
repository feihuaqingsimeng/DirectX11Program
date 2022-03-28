#pragma once
#include <windows.h>
#include "resource.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "dx11/DXException.h"
#include <optional>
#include "dx11/Graphics.h"
#include <memory>
class Window
{
private:
	class WindowClass {
	public:
		static const wchar_t* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const wchar_t* wndClassName = L"Direct3D Engine Window";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
	
public:
	Window(int width, int height, const wchar_t* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::wstring& name);
	static std::optional<int> ProcessMessages();
	Graphics& Gfx();

private:
	static LRESULT CALLBACK HandleMsgSetUp(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
public:
	Keyboard keyboard;
	Mouse mouse;
private:
	int width;
	int height;
	HWND hwnd; 
	std::unique_ptr<Graphics> graphics;
};