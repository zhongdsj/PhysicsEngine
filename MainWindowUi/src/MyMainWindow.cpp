# include <MyMainWindow.h>

# include <Context.h>
# include <MyWindowClass.h>

ZDSJ::MyMainWindow::MyMainWindow(DWORD _ex_style, LPCWSTR _class_name, LPCWSTR _window_name, DWORD _style, int _x,
                                 int _y, int _width, int _height, HWND _parent, HMENU _menu, HINSTANCE _instance)
{
	this->m_windowClass = new ZDSJ::MyWindowClass(_instance, _class_name);
	this->m_handle = CreateWindowExW(
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
	ShowWindow(this->m_handle, SW_SHOWDEFAULT);
}

LRESULT ZDSJ::MyMainWindow::handelMessage(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param)
{
	// todo 处理消息
	for (auto message_handler : this->m_messageHandler)
	{
		if(message_handler.second(handle, msg, w_param, l_param))
		{
			return true;
		}
	}
	switch (msg)
	{
	case WM_MOVE:
		{
			const int x_pos = (int)static_cast<short>(LOWORD(l_param));   // horizontal position 
			const int y_pos = (int)static_cast<short>(HIWORD(l_param));   // vertical position 
			Context_Instance->setWindowX(x_pos);
			Context_Instance->setWindowY(y_pos);
			break;
		}
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProc(handle, msg, w_param, l_param);
}

void ZDSJ::MyMainWindow::addHandelMessage(const char* _id, std::function<LRESULT(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param)> _handler)
{
	this->m_messageHandler.insert(std::make_pair(_id, _handler));
}

bool ZDSJ::MyMainWindow::getMessage(MSG& _msg)
{
	// 处理消息, 退出/异常消息返回false，外部视情况处理
	BOOL res = PeekMessage(&_msg, nullptr, 0U, 0U, PM_REMOVE);
	if(res == 0)
	{
		return true;
	}
	if (_msg.message == WM_QUIT)
	{
		return false;
	}
	TranslateMessage(&_msg);
	DispatchMessageW(&_msg);
	//switch (res)
	//{
	//case 0:
	//	// log quit
	//	return false;
	//case -1:
	//	// log err
	//	return false;
	//default:
	//	// 非0 非-1 为正常消息
	//	TranslateMessage(&_msg);
	//	DispatchMessageW(&_msg);
	//	break;
	//}
	return true;
}

HWND ZDSJ::MyMainWindow::getHandle() const
{
	return this->m_handle;
}

ZDSJ::MyMainWindow::~MyMainWindow()
{
	DestroyWindow(this->m_handle);
	Log_Info("[win api]: window destroy");
}

ZDSJ::ApplicationWindowInterface* ZDSJ::createWindow(const wchar_t* _window_name, int _x, int _y, int _width, int _height)
{
	Log_Info("[win api]: create window");
	constexpr DWORD ex_style = NULL;
	const auto class_name = L"ZDSJWindow";
	constexpr DWORD style = WS_SYSMENU;
	const HWND parent = nullptr;
	const HMENU menu = nullptr;
	const HINSTANCE instance = GetModuleHandle(nullptr);
	return new MyMainWindow(ex_style, class_name, _window_name, style, _x, _y, _width, _height, parent, menu, instance);
}
