# pragma once
# include <Windows.h>
# include "CommonMacro.h"
# include "MainWindowUiExport.h"
# include "MyWindowInterface.h"
# include "ApplicationWindowInterface.h"

namespace ZDSJ
{
	class MyWindowClass;
	class Main_Window_Ui_Api MyMainWindow: public MyWindowInterface, public ApplicationWindowInterface
	{
	public:
		MyMainWindow(DWORD _ex_style, LPCWSTR _class_name, LPCWSTR _window_name, DWORD _style, int _x, int _y, int _width, int _height,
		             HWND _parent, HMENU _menu, HINSTANCE _instance);
		LRESULT handelMessage(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param) override;
		bool getMessage(MSG& _msg) override;
		~MyMainWindow() override;

		Delete_Copy_And_Move(MyMainWindow);
	private:
		HWND m_handle = nullptr;
		MyWindowClass* m_windowClass = nullptr;
	};

	// ZDSJ::ApplicationWindowInterface* createWindow(LPCWSTR _window_name, int _x, int _y, int _width, int _height);
}
