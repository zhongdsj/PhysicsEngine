#pragma once
# include <Windows.h>
namespace ZDSJ {

	class MyWindowInterface {
	public:
		virtual bool create(DWORD _ex_style, LPCWSTR _class_name, LPCWSTR _window_name, DWORD _style, int _x, int _y, int _width, int _height,
			HWND _parent, HMENU _menu, HINSTANCE _instance) = 0;
		virtual LRESULT handelMessage(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	};

}
