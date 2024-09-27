#pragma once
# include <MyWindowInterface.h>

namespace ZDSJ {
	class MyDx11;
	class MyWindow : public MyWindowInterface
	{
	public:
		MyWindow() = default;
		HWND getHwnd();
		bool create(DWORD _ex_style, LPCWSTR _class_name, LPCWSTR _window_name, DWORD _style, int _x, int _y, int _width, int _height,
			HWND _parent, HMENU _menu, HINSTANCE _instance) override;
		LRESULT handelMessage(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
		void doFrame(short _fps);
		~MyWindow();
		MyWindow(const MyWindow&) = delete;
		MyWindow& operator=(const MyWindow&) = delete;
	private:
		HWND m_hwnd = nullptr;
		MyDx11* m_dx11 = nullptr;
	};
}