﻿#include "MyWindow.h"
#include <MyDx11/MyDx11.h>

# pragma region MyWindow

HWND ZDSJ::MyWindow::getHwnd()
{
	return this->m_hwnd;
}

bool ZDSJ::MyWindow::create(DWORD _ex_style, LPCWSTR _class_name, LPCWSTR _window_name, DWORD _style, int _x, int _y, int _width, int _height, HWND _parent, HMENU _menu, HINSTANCE _instance)
{
	this->m_hwnd = CreateWindowExW(
		_ex_style,
		// WS_EX_TOPMOST: 置顶, 调试ui时可用
		// WS_EX_ACCEPTFILES: 允许拖拽文件
		// WS_EX_TOOLWINDOW: 该窗口打算用作浮动工具栏,不会被alt+tab呼出,如果有菜单则右键呼出
		// WS_EX_NOACTIVATE: 可以让窗体被点击后 也不激活，不抢夺焦点，同时窗口上的输入框也不会默认获得输入焦点
		// WS_EX_LAYERED
		// WS_EX_TRANSPARENT
		// WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP
		_class_name,                     // Window class
		_window_name,    // Window text
		_style, // WS_POPUPWINDOW: 没有标题栏
		_x, _y, _width, _height, _parent, _menu, _instance, this
	);
	if (this->m_hwnd == nullptr) {
		auto err = GetLastError();
		return false;
	}
	this->m_dx11 = new ZDSJ::MyDx11(this->m_hwnd, _width, _height);
	ShowWindow(this->m_hwnd, SW_SHOWDEFAULT);

	return true;
}

LRESULT ZDSJ::MyWindow::handelMessage(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProc(handle, msg, wParam, lParam);
}

void ZDSJ::MyWindow::doFrame(short _fps)
{
	this->m_dx11->render(_fps);
}

ZDSJ::MyWindow::~MyWindow()
{
	delete this->m_dx11;
	DestroyWindow(this->m_hwnd);
}

# pragma endregion MyWindow
