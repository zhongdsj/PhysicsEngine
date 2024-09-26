#pragma once
# include <Windows.h>

namespace ZDSJ {

	class MyWindowInterface;
	class MyWindowClass {
	public:
		MyWindowClass(HINSTANCE _instance, LPCWSTR _class_name, UINT _style = CS_DBLCLKS, HICON _icon = NULL, HICON _icon_sm = nullptr, HCURSOR _cursor = nullptr, LPCWSTR _menue_name = nullptr, int _cls_extra = 0, int _wnd_extra = 0, HBRUSH _background = nullptr);
		bool creatWindow(MyWindowInterface* _window, int _x, int _y, int _width, int _height, LPCWSTR _window_name, HWND _parent = nullptr,
			DWORD _ex_style = NULL, DWORD _style = WS_SYSMENU, HMENU _menu = nullptr);
		~MyWindowClass();
	private:
		static LRESULT CALLBACK handelMessageSetUp(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK handelMessageForward(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
		WNDCLASSEXW m_window_class = { 0 };
	};

}