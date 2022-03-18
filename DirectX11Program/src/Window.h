#pragma once
#include "../framework.h"
#include "DXException.h"
class Window
{
public:	
	class Exception :public DXException {
	public:
		Exception(int line, const char* file, HRESULT hr);
		const char* what() const override;
		virtual const char* GetType() const;
		static std::string TranslateErrorCode(HRESULT hr);
		HRESULT GetErrorCode() const;
		std::string GetErrorString() const;
	private:
		HRESULT hr;
	};
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
private:
	static LRESULT CALLBACK HandleMsgSetUp(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
private:
	int width;
	int height;
	HWND hwnd;
};

#define  CHWND_EXCEPT(hr) Window::Exception(__LINE__,__FILE__,hr)
#define  CHWND_LAST_EXCEPT() Window::Exception(__LINE__,__FILE__,GetLastError());