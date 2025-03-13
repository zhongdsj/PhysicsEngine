# include <MyWindowClass.h>
# include <MyWindowInterface.h>

ZDSJ::MyWindowClass::MyWindowClass(HINSTANCE _instance, LPCWSTR _class_name, UINT _style, HICON _icon, HICON _icon_sm, HCURSOR _cursor, LPCWSTR _menu_name, int _cls_extra, int _wnd_extra, HBRUSH _background)
{
	this->m_window_class.cbSize = sizeof(WNDCLASSEXW);
	this->m_window_class.lpfnWndProc = ZDSJ::MyWindowClass::handelMessageSetUp;
	this->m_window_class.hInstance = _instance;
	this->m_window_class.lpszClassName = _class_name;
	this->m_window_class.style = _style;
	this->m_window_class.hIcon = _icon;
	this->m_window_class.hIconSm = _icon_sm;
	this->m_window_class.hCursor = _cursor;
	this->m_window_class.lpszMenuName = _menu_name;
	this->m_window_class.cbClsExtra = _cls_extra;
	this->m_window_class.cbWndExtra = _wnd_extra;
	this->m_window_class.hbrBackground = _background;
	// 注册窗口类
	auto res = RegisterClassExW(&this->m_window_class);
}

ZDSJ::MyWindowClass::~MyWindowClass()
{
	if (this->m_window_class.cbSize != 0) {
		UnregisterClassW(this->m_window_class.lpszClassName, this->m_window_class.hInstance);
	}
}

LRESULT ZDSJ::MyWindowClass::handelMessageSetUp(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param)
{
	if (msg == WM_NCCREATE) {
		// 创建窗口时触发
		// lParam 指向 CREATESTRUCT 结构的指针，其中包含有关正在创建的窗口的信息。
		// CREATESTRUCT.lpCreateParams 也就是CreateWindowExW的LPVOID指向的指针
		const CREATESTRUCTW* const p_create = reinterpret_cast<CREATESTRUCTW*>(l_param);
		ZDSJ::MyWindowInterface* const pwnd = static_cast<ZDSJ::MyWindowInterface*>(p_create->lpCreateParams);
		// 将执行CreateWindowExW的this指针存入窗口
		SetWindowLongPtrW(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pwnd));
		// 不能通过WINAPI调用成员函数, 在将此函数设置为消息处理函数后，被当作WINAPI了
		// 将消息转发到静态方法
		// GWLP_WNDPROC设置窗口过程的新地址
		SetWindowLongPtrW(handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&ZDSJ::MyWindowClass::handelMessageForward));
		return pwnd->handelMessage(handle, msg, w_param, l_param);
	}
	return DefWindowProc(handle, msg, w_param, l_param);
}

LRESULT ZDSJ::MyWindowClass::handelMessageForward(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param)
{
	ZDSJ::MyWindowInterface* const pwnd = reinterpret_cast<ZDSJ::MyWindowInterface*>(GetWindowLongPtrW(handle, GWLP_USERDATA));
	return pwnd->handelMessage(handle, msg, w_param, l_param);
}