# pragma once
# include <Windows.h>
# include "CommonMacro.h"

namespace ZDSJ {

	class MyWindowClass {
	public:
		MyWindowClass(HINSTANCE _instance, LPCWSTR _class_name, UINT _style = CS_DBLCLKS, HICON _icon = nullptr, HICON _icon_sm = nullptr, HCURSOR _cursor = nullptr, LPCWSTR _menu_name = nullptr, int _cls_extra = 0, int _wnd_extra = 0, HBRUSH _background = nullptr);
		~MyWindowClass();

		Delete_Copy_And_Move(MyWindowClass);
	private:
		static LRESULT CALLBACK handelMessageSetUp(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param);
		static LRESULT CALLBACK handelMessageForward(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param);
		WNDCLASSEXW m_window_class = { 0 };
	};

}